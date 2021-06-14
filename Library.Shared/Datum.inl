#include "Datum.h"

namespace FieaGameEngine
{
	template<typename IncrementFunctor>
	inline void Datum::PushBackFromString(const std::string& str)
	{
		if (_type == DatumType::Unknown || _type == DatumType::Pointer)
		{
			throw std::runtime_error("Cannot SetFromString with an unknown or pointer type");
		}
		PushBackFromStringFunctor func = _pushBackFromStringFunctors[static_cast<int>(_type)];
		assert(func != nullptr);
		(this->*func)(str);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const int& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Integer, incrementFunctor);
		new(_data.i + _size++)int(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const float& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Float, incrementFunctor);
		new(_data.f + _size++)float(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const glm::vec4& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Vector, incrementFunctor);
		new(_data.v + _size++)glm::vec4(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const glm::mat4& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Matrix, incrementFunctor);
		new(_data.m + _size++)glm::mat4(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(Scope& value, IncrementFunctor incrementFunctor)
	{
		SetType(DatumType::Table);
		PushBackHelper(DatumType::Table, incrementFunctor);
		new(_data.t + _size++)Scope*(&value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const std::string& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::String, incrementFunctor);
		new(_data.s + _size++)std::string(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(RTTI* const& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Pointer, incrementFunctor);
		new(_data.p + _size++)RTTI* (value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(int&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Integer, incrementFunctor);
		new(_data.i + _size++)int(std::forward<int>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(float&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Float, incrementFunctor);
		new(_data.f + _size++)float(std::forward<float>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(glm::vec4&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Vector, incrementFunctor);
		new(_data.v + _size++)glm::vec4(std::forward<glm::vec4>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(glm::mat4&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Matrix, incrementFunctor);
		new(_data.m + _size++)glm::mat4(std::forward<glm::mat4>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(std::string&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::String, incrementFunctor);
		new(_data.s + _size++)std::string(std::forward<std::string>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(RTTI*&& value, IncrementFunctor incrementFunctor)
	{
		PushBackHelper(DatumType::Pointer, incrementFunctor);
		new(_data.p + _size++)RTTI* (std::forward<RTTI*>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBackHelper(DatumType type, IncrementFunctor incrementFunctor)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external storage.");
		}
		if (_type != type)
		{
			throw std::runtime_error("Cannot PushBack with an unknown type or a different type.");
		}
		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunctor(_size, _capacity));
			Reserve(capacity);
		}
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(const int& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(const float& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(const glm::vec4& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(const glm::mat4& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(const std::string& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline void Datum::Remove(RTTI* const& value, EqualityFunctor equalityFunctor)
	{
		return RemoveAt(IndexOf(value, equalityFunctor));
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(const int& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::Integer)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.i[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(const float& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::Float)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.f[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(const glm::vec4& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::Vector)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.v[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(const glm::mat4& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::Matrix)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.m[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(const std::string& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::String)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.s[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline size_t Datum::IndexOf(RTTI* const& value, EqualityFunctor equalityFunctor)
	{
		if (_type != DatumType::Pointer)
		{
			throw std::runtime_error("Cannot IndexOf with an unknown type or a different type.");
		}
		size_t index = 0_z;
		for (; index != _size; ++index)
		{
			if (equalityFunctor(_data.p[index], value))
			{
				break;
			}
		}

		return index;
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(const int& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(const float& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(const glm::vec4& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(const glm::mat4& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(const std::string& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	template<typename EqualityFunctor>
	inline const size_t Datum::IndexOf(RTTI* const& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Datum*>(this)->IndexOf(value, equalityFunctor);
	}

	inline const size_t Datum::_sizeMap[] =
	{
		sizeof(int),			// DatumType::Integer
		sizeof(float),			// DatumType::Float
		sizeof(glm::vec4),		// DatumType::Vector
		sizeof(glm::mat4),		// DatumType::Matrix
		sizeof(Scope*),			// DatumType::Table
		sizeof(std::string),	// DatumType::String
		sizeof(RTTI*)			// DatumType::Pointer
	};

	inline const HashMap<std::string, Datum::DatumType>  Datum::DatumTypeMap =
	{
		std::make_pair("integer", DatumType::Integer),	// DatumType::Integer
		std::make_pair("float", DatumType::Float),		// DatumType::Float
		std::make_pair("vector", DatumType::Vector),		// DatumType::Vector
		std::make_pair("matrix", DatumType::Matrix),		// DatumType::Matrix
		std::make_pair("table", DatumType::Table),		// DatumType::Table
		std::make_pair("string", DatumType::String),	// DatumType::String
		std::make_pair("pointer", DatumType::Pointer),	// DatumType::Pointer
	};

	inline void Datum::CreateIntegers(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.i + i)int(0);
		}
	}

	inline void Datum::CreateFloats(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.f + i)float(0.f);
		}
	}

	inline void Datum::CreateVectors(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.v + i)glm::vec4(0);
		}
	}

	inline void Datum::CreateMatrixes(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.m + i)glm::mat4(0);
		}
	}

	inline void Datum::CreateStrings(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.s + i)std::string();
		}
	}

	inline void Datum::CreatePointers(size_t start, size_t size) const
	{
		for (size_t i = start; i < size; ++i)
		{
			new (_data.p + i)RTTI* (nullptr);
		}
	}

	inline const Datum::CreateFunctor Datum::_createFunctors[] =
	{
		&Datum::CreateIntegers,		// DatumType::Integer
		&Datum::CreateFloats,		// DatumType::Float
		&Datum::CreateVectors,		// DatumType::Vector
		&Datum::CreateMatrixes,		// DatumType::Matrix
		&Datum::CreatePointers,		// DatumType::Table
		&Datum::CreateStrings,		// DatumType::String
		&Datum::CreatePointers,		// DatumType::Pointer
		nullptr,
	};

	inline void Datum::CopyIntegers(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.i + i)int(*(data.i + i));
		}
	}

	inline void Datum::CopyFloats(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.f + i)float(*(data.f + i));
		}
	}

	inline void Datum::CopyVectors(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.v + i)glm::vec4(*(data.v + i));
		}
	}

	inline void Datum::CopyMatrixes(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.m + i)glm::mat4(*(data.m + i));
		}
	}

	inline void Datum::CopyStrings(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.s + i)std::string(*(data.s + i));
		}
	}

	inline void Datum::CopyPointers(Datum::DatumValue data, size_t size) const
	{
		for (size_t i = 0_z; i < size; ++i)
		{
			new (_data.p + i)RTTI* (*(data.p + i));
		}
	}

	inline const Datum::CopyFunctor Datum::_copyFunctors[] =
	{
		&Datum::CopyIntegers,		// DatumType::Integer
		&Datum::CopyFloats,			// DatumType::Float
		&Datum::CopyVectors,		// DatumType::Vector
		&Datum::CopyMatrixes,		// DatumType::Matrix
		&Datum::CopyPointers,		// DatumType::Table
		&Datum::CopyStrings,		// DatumType::String
		&Datum::CopyPointers,		// DatumType::Pointer
		nullptr,
	};

	inline bool Datum::ComparePODs(const Datum& rhs) const
	{
		return memcmp(_data.vp, rhs._data.vp, _sizeMap[static_cast<int>(_type)] * _size) == 0;
	}

	inline bool Datum::CompareStrings(const Datum& rhs) const
	{
		bool areEqual = true;
		for (size_t i = 0_z; i < _size; ++i)
		{
			if (_data.s[i].compare(rhs._data.s[i]) != 0) {
				areEqual = false;
				break;
			}
		}
		return areEqual;
	}

	inline bool Datum::ComparePointers(const Datum& rhs) const
	{
		bool areEqual = true;
		for (size_t i = 0_z; i < _size; ++i)
		{
			RTTI* lhsPointer = _data.p[i];
			RTTI* rhsPointer = rhs._data.p[i];

			if (lhsPointer != rhsPointer && lhsPointer != nullptr && !lhsPointer->Equals(rhsPointer))
			{
				areEqual = false;
				break;
			}
		}
		return areEqual;
	}

	inline const Datum::CompareFunctor Datum::_compareFunctors[] =
	{
		&Datum::ComparePODs,		// DatumType::Integer
		&Datum::ComparePODs,		// DatumType::Float
		&Datum::ComparePODs,		// DatumType::Vector
		&Datum::ComparePODs,		// DatumType::Matrix
		&Datum::ComparePointers,	// DatumType::Table
		&Datum::CompareStrings,		// DatumType::String
		&Datum::ComparePointers,	// DatumType::Pointer
		nullptr,
	};

	inline void Datum::SetFromStringIntegers(const std::string& str, size_t index)
	{
		Set(FromStringToInt(str), index);
	}

	inline void Datum::SetFromStringFloats(const std::string& str, size_t index)
	{
		Set(FromStringToFloat(str), index);
	}

	inline void Datum::SetFromStringVectors(const std::string& str, size_t index)
	{
		Set(FromStringToVector(str), index);
	}

	inline void Datum::SetFromStringMatrixes(const std::string& str, size_t index)
	{
		Set(FromStringToMatrix(str), index);
	}

	inline void Datum::SetFromStringStrings(const std::string& str, size_t index)
	{
		Set(str, index);
	}

	inline const Datum::SetFromStringFunctor Datum::_setFromStringFunctors[] =
	{
		&Datum::SetFromStringIntegers,		// DatumType::Integer
		&Datum::SetFromStringFloats,		// DatumType::Float
		&Datum::SetFromStringVectors,		// DatumType::Vector
		&Datum::SetFromStringMatrixes,		// DatumType::Matrix
		nullptr,
		&Datum::SetFromStringStrings,		// DatumType::String
		nullptr,							// DatumType::Pointer
		nullptr,
	};

	inline std::string Datum::ToStringIntegers(size_t index) const
	{
		return std::to_string(GetInteger(index));
	}

	inline std::string Datum::ToStringFloats(size_t index) const
	{
		return std::to_string(GetFloat(index));
	}

	inline std::string Datum::ToStringVectors(size_t index) const
	{
		return glm::to_string(GetVector(index));
	}

	inline std::string Datum::ToStringMatrixes(size_t index) const
	{
		return glm::to_string(GetMatrix(index));
	}

	inline std::string Datum::ToStringStrings(size_t index) const
	{
		return GetString(index);
	}

	inline std::string Datum::ToStringPointers(size_t index) const
	{
		return GetPointer(index)->ToString();
	}

	inline const Datum::ToStringFunctor Datum::_toStringFunctors[] =
	{
		&Datum::ToStringIntegers,		// DatumType::Integer
		&Datum::ToStringFloats,			// DatumType::Float
		&Datum::ToStringVectors,		// DatumType::Vector
		&Datum::ToStringMatrixes,		// DatumType::Matrix
		nullptr,
		&Datum::ToStringStrings,		// DatumType::String
		&Datum::ToStringPointers,		// DatumType::Pointer
		nullptr,
	};

	inline void Datum::PushBackFromStringIntegers(const std::string& str)
	{
		PushBack(FromStringToInt(str));
	}

	inline void Datum::PushBackFromStringFloats(const std::string& str)
	{
		PushBack(FromStringToFloat(str));
	}

	inline void Datum::PushBackFromStringVectors(const std::string& str)
	{
		PushBack(FromStringToVector(str));
	}

	inline void Datum::PushBackFromStringMatrixes(const std::string& str)
	{
		PushBack(FromStringToMatrix(str));
	}

	inline void Datum::PushBackFromStringStrings(const std::string& str)
	{
		PushBack(str);
	}

	inline const Datum::PushBackFromStringFunctor Datum::_pushBackFromStringFunctors[] =
	{
		&Datum::PushBackFromStringIntegers,		// DatumType::Integer
		&Datum::PushBackFromStringFloats,		// DatumType::Float
		&Datum::PushBackFromStringVectors,		// DatumType::Vector
		&Datum::PushBackFromStringMatrixes,		// DatumType::Matrix
		nullptr,
		&Datum::PushBackFromStringStrings,		// DatumType::String
		nullptr,							// DatumType::Pointer
		nullptr,
	};
}
