#include "pch.h"

#include "Attributed.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType typeId)
	{
		(*this)["this"s] = this;
		Populate(typeId);
	}

	void Attributed::Populate(RTTI::IdType typeId)
	{
		const Vector<Signature>& signatures = TypeManager::GetSignaturesForType(typeId);

		for (const Signature& signature : signatures)
		{
			assert(signature.Type != Datum::DatumType::Unknown);
			Datum& datum = Append(signature.Name);

			if (signature.Type == Datum::DatumType::Table)
			{
				datum.SetType(Datum::DatumType::Table);
				if (signature.Size > 0)
				{
					datum.Reserve(signature.Size);
					for (size_t i = 0; i < signature.Size; ++i)
					{
						AppendScope(signature.Name);
					}
				}
			}
			else
			{
				void* data = reinterpret_cast<uint8_t*>(this) + signature.Offset;
				datum.SetStorage(signature.Type, data, signature.Size);
			}
		}
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other)
	{
		(*this)["this"s] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other))
	{
		(*this)["this"s] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		Scope::operator=(other);
		(*this)["this"s] = this;
		UpdateExternalStorage(other.TypeIdInstance());
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		Scope::operator=(std::move(other));
		(*this)["this"s] = this;
		UpdateExternalStorage(other.TypeIdInstance());
		return *this;
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeId)
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(typeId);
		size_t prescribedSignatureCount = signatures.Size() + 1; // +1 for "this"

		for (size_t i = 1; i < prescribedSignatureCount; ++i)
		{
			const Signature& signature = signatures[i - 1];
			auto& [key, datum] = *(_orderVector[i]);

			assert(signature.Name == key);
			assert(signature.Type == datum.Type());

#if defined(NDEBUG)
			UNREFERENCED(key);
#endif	

			if (signature.Type != Datum::DatumType::Table)
			{
				assert(datum.IsExternal());
				assert(datum.Size() == signature.Size);
				void* data = reinterpret_cast<uint8_t*>(this) + signature.Offset;
				datum.SetStorage(signature.Type, data, signature.Size);
			}
		}
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return _map.ContainsKey(name);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		if (name == "this"s)
		{
			return true;
		}

		auto signatures = TypeManager::GetSignaturesForType(TypeIdInstance());
		for (auto signature : signatures)
		{
			if (signature.Name == name)
			{
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::invalid_argument("Cannot Append an already Prescribed Attribute");
		}
		return Scope::Append(name);
	}

	const Vector<Scope::PairType*>& Attributed::Attributes() const
	{
		return _orderVector;
	}

	Vector<Scope::PairType*> Attributed::PrescribedAttributes() const
	{
		const auto signatures = TypeManager::GetSignaturesForType(TypeIdInstance());

		size_t prescribedAttributeCount = signatures.Size() + 1; // +1 for the "this" attribute
		Vector<PairType*> prescribedAttributes(prescribedAttributeCount);
		for (size_t i = 0; i < prescribedAttributeCount; ++i)
		{
			prescribedAttributes.PushBack(_orderVector[i]);
		}

		return prescribedAttributes;
	}

	Vector<Scope::PairType*> Attributed::AuxiliaryAttributes() const
	{
		const auto signatures = TypeManager::GetSignaturesForType(TypeIdInstance());

		size_t auxiliaryAttributeBeginIndex = signatures.Size() + 1; // +1 for the "this" attribute
		Vector<PairType*> auxiliaryAttributes(_orderVector.Size() - auxiliaryAttributeBeginIndex);
		for (size_t i = auxiliaryAttributeBeginIndex; i < _orderVector.Size(); ++i)
		{
			auxiliaryAttributes.PushBack(_orderVector[i]);
		}

		return auxiliaryAttributes;
	}

}