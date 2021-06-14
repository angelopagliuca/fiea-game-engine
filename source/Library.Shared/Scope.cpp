#include "pch.h"

#include "Scope.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(size_t size) :
		_orderVector{size}
	{
	}

	Scope::Scope(const Scope& other)
	{
		DeepCopy(other);
	}

	Scope::Scope(Scope&& other) noexcept :
		_parent{ other._parent }, _map{ std::move(other._map) }, _orderVector{ std::move(other._orderVector) }
	{
		Reparent(std::move(other));
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Scope:: Clear();
			DeepCopy(other);
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (this != &other)
		{
			Scope::Clear();
			
			_parent = other._parent;
			_map = std::move(other._map);
			_orderVector = std::move(other._orderVector);

			Reparent(std::move(other));
		}

		return *this;
	}

	Scope::~Scope()
	{
		Scope::Clear();
	}

	void Scope::DeepCopy(const Scope& other)
	{
		_orderVector.Reserve(other._orderVector.Size());

		for (const auto& pair : other._orderVector)
		{
			Datum& existingDatum = pair->second;
			Datum& newDatum = Append(pair->first);
			
			if (existingDatum.Type() == Datum::DatumType::Table)
			{
				assert(!existingDatum.IsExternal());
				newDatum.SetType(Datum::DatumType::Table);
				newDatum.Reserve(existingDatum.Size());

				for (size_t datumIndex = 0; datumIndex < existingDatum.Size(); ++datumIndex)
				{
					Scope& existingScope = existingDatum.GetScope(datumIndex);
					Scope* scope = existingScope.Clone();
					scope->_parent = this;
					newDatum.PushBack(*scope);
				}
			}
			else
			{
				newDatum = existingDatum;
			}
		}
	}


	bool Scope::operator==(const Scope& other) const
	{
		if (this != &other)
		{
			if (_orderVector.Size() != other._orderVector.Size())
			{
				return false;
			}

			for (size_t i = 0_z; i < _orderVector.Size(); ++i)
			{
				auto& lhsPair = *_orderVector[i];
				auto& rhsPair = *other._orderVector[i];

				if (lhsPair.first == "this"s)
				{
					continue;
				}

				if (lhsPair != rhsPair)
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Datum& Scope::operator[](size_t index)
	{
		return _orderVector[index]->second;
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return _orderVector[index]->second;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	size_t Scope::Size() const
	{
		return _orderVector.Size();
	}

	bool Scope::IsEmpty() const
	{
		return _orderVector.Size() == 0;
	}

	void Scope::Clear()
	{
		Orphan();

		ForEachNestedScopeIn([](const Scope& parent, Datum& datum, size_t datumIndex)
		{
			Scope& scope = datum[datumIndex];
			assert(scope._parent == &parent);

#ifdef NDEBUG
			UNREFERENCED(parent);
#endif // NDEBUG
			scope._parent = nullptr;
			delete &scope;
			return false;
		});

		_map.Clear();
		_orderVector.Clear();
	}

	Scope* Scope::GetParent()
	{
		return _parent;
	}

	const Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::Append(const std::string& name)
	{
		bool entryCreated;
		return Append(name, entryCreated);
	}

	Datum& Scope::Append(const std::string& name, bool& entryCreated)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Cannot Append an empty string name into Scope");
		}

		auto [it, wasInserted] = _map.Insert(std::make_pair(name, Datum()));
		entryCreated = wasInserted;

		if (entryCreated)
		{
			_orderVector.PushBack(&*it);
		}
		return it->second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		bool entryCreated;
		Datum& datum = Append(name, entryCreated);

		if (!entryCreated)
		{
			if (datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
			{
				throw std::runtime_error("Cannot Append a Scope to a Datum with invalid type");
			}

			assert(!datum._isExternal);
		}

		Scope* scope = new Scope;
		scope->_parent = this;
		datum.PushBack(*scope);

		return *scope;
	}

	const std::pair<Datum*, size_t> Scope::FindContainedScope(const Scope& scope) const
	{
		Datum* foundDatum = nullptr;
		size_t foundIndex = numeric_limits<size_t>::max();

		ForEachNestedScopeIn([&scope, &foundDatum, &foundIndex](const Scope&, Datum& datum, size_t datumIndex)
		{
			if (&datum.GetScope(datumIndex) == &scope)
			{
				foundDatum = &datum;
				foundIndex = datumIndex;
				return true;
			}
			return false;
		});

		return std::make_pair(foundDatum, foundIndex);
	}

	Datum* Scope::Find(const std::string& name)
	{
		const Datum* datum = const_cast<const Scope&>(*this).Find(name);
		return const_cast<Datum*>(datum);
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		auto it = _map.Find(name);
		return it != _map.end() ? &it->second : nullptr;
	}

	Datum* Scope::Search(const std::string& name)
	{
		Scope* found;
		return Search(name, found);
	}

	const Datum* Scope::Search(const std::string& name) const
	{
		const Scope* found;
		return Search(name, found);
	}

	Datum* Scope::Search(const std::string& name, Scope*& foundScope)
	{
		const Datum* datum = const_cast<const Scope&>(*this).Search(name, const_cast<const Scope*&>(foundScope));
		return const_cast<Datum*>(datum);
	}

	const Datum* Scope::Search(const std::string& name, const Scope*& foundScope) const
	{
		const Datum* datum = Find(name);
		foundScope = this;
		if (datum == nullptr)
		{
			foundScope = nullptr;
			if (_parent != nullptr)
			{
				datum = _parent->Search(name, foundScope);
			}
		}
		return datum;
	}

	Datum& Scope::At(const std::string& name)
	{
		Datum* datum = Find(name);
		assert(datum != nullptr);
		return *datum;
	}

	const Datum& Scope::At(const std::string& name) const
	{
		const Datum* datum = Find(name);
		assert(datum != nullptr);
		return *datum;
	}

	void Scope::Adopt(Scope& scope, const std::string& name)
	{
		if (this == &scope)
		{
			throw std::runtime_error("Cannot self-Adopt");
		}
		if (scope.IsAncestorOf(*this))
		{
			throw std::runtime_error("Cannot Adopt ancestor Scope");
		}

		bool entryCreated;
		Datum& datum = Append(name, entryCreated);
		if (!entryCreated)
		{
			if (datum.Type() != Datum::DatumType::Unknown && datum.Type() != Datum::DatumType::Table)
			{
				throw std::runtime_error("Cannot Append a Scope to a Datum with invalid type");
			}

			assert(!datum._isExternal);
		
		}

		scope.Orphan();
		datum.PushBack(scope);
		scope._parent = this;
	}

	void Scope::Orphan()
	{
		if (_parent != nullptr)
		{
			auto [datum, index] = _parent->FindContainedScope(*this);
			assert(datum != nullptr);
			datum->RemoveAt(index);
			_parent = nullptr;
		}
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		const Scope* other = rhs->As<Scope>();
		return other != nullptr ? *this == *other : false;
	}
	std::string Scope::ToString() const
	{
		std::string str = "Scope(";
		str.append(std::to_string(Size()));
		str.append(")->ToString");
		return str;
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool isAncestorOf = false;

		Scope* parent = scope._parent;
		while (parent != nullptr)
		{
			if (parent == this)
			{
				isAncestorOf = true;
				break;
			}
			parent = parent->_parent;
		}
		return isAncestorOf;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	void Scope::Reparent(Scope&& other)
	{
		if (other._parent != nullptr)
		{
			// Move prev parent's Scope reference to "this"
			auto [datum, datumIndex] = other._parent->FindContainedScope(other);
			assert(datum != nullptr);

			datum->Set(*this, datumIndex);
			other._parent = nullptr;
		}

		// Move child Scope parent references to "this"
		ForEachNestedScopeIn([](const Scope& parent, Datum& datum, size_t datumIndex)
		{
			Scope& childScope = datum.GetScope(datumIndex);
			childScope._parent = &(const_cast<Scope&>(parent));
			return false;
		});
	}

	void Scope::ForEachNestedScopeIn(NestedScopeFunction func) const
	{
		for (const auto& pair : _orderVector)
		{
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				assert(!datum.IsExternal());

				for (size_t datumIndex = 0; datumIndex < datum.Size(); ++datumIndex)
				{
					if (func(*this, datum, datumIndex))
					{
						return;
					}
				}
			}
		}
	}
}