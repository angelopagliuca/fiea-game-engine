#include "pch.h"

#include "Datum.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{
	Datum::Datum(Datum::DatumType type) :
		_type(type)
	{
	}

	Datum::Datum(std::initializer_list<int> list) :
		Datum(Datum::DatumType::Integer)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.i + _size++)int(value);
		}
	}

	Datum::Datum(std::initializer_list<float> list) :
		Datum(Datum::DatumType::Float)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.f + _size++)float(value);
		}
	}

	Datum::Datum(std::initializer_list<vec4> list) :
		Datum(Datum::DatumType::Vector)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.v + _size++)vec4(value);
		}
	}

	Datum::Datum(std::initializer_list<mat4> list) :
		Datum(Datum::DatumType::Matrix)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.m + _size++)mat4(value);
		}
	}

	Datum::Datum(std::initializer_list<string> list) :
		Datum(Datum::DatumType::String)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.s + _size++)string(value);
		}
	}

	Datum::Datum(std::initializer_list<RTTI*> list) :
		Datum(Datum::DatumType::Pointer)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.p + _size++)RTTI*(value);
		}
	}

	Datum::Datum(const int& value) :
		Datum(Datum::DatumType::Integer)
	{
		Resize(1_z);
		*_data.i = value;
	}

	Datum::Datum(const float& value) :
		Datum(Datum::DatumType::Float)
	{
		Resize(1_z);
		*_data.f = value;
	}

	Datum::Datum(const glm::vec4& value) :
		Datum(Datum::DatumType::Vector)
	{
		Resize(1_z);
		*_data.v = value;
	}

	Datum::Datum(const glm::mat4& value) :
		Datum(Datum::DatumType::Matrix)
	{
		Resize(1_z);
		*_data.m = value;
	}

	Datum::Datum(const std::string& value) :
		Datum(Datum::DatumType::String)
	{
		Resize(1_z);
		*_data.s = value;
	}

	Datum::Datum(RTTI*& value) :
		Datum(Datum::DatumType::Pointer)
	{
		Resize(1_z);
		*_data.p = value;
	}

	Datum::Datum(const Datum& other) :
		_type(other._type), _size(other._size), _isExternal(other._isExternal)
	{
		if (other._size > 0)
		{
			if (other._isExternal)
			{
				_data = other._data;
				_capacity = other._capacity;
			}
			else
			{
				Reserve(other._capacity);
				CopyFunctor func = _copyFunctors[static_cast<int>(_type)];
				assert(func != nullptr);
				(this->*func)(other._data, _size);
			}
		}
	}

	Datum::Datum(Datum&& other) noexcept :
		_type(other._type), _data(other._data), _size(other._size), _capacity(other._capacity)
	{
		other._type = DatumType::Unknown;
		other._data.vp = nullptr;
		other._size = 0_z;
		other._capacity = 0_z;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			_type = other._type;
			_isExternal = other._isExternal;
			if (other._isExternal)
			{
				_data = other._data;
				_capacity = other._capacity;
			}
			else
			{
				Clear();
				Reserve(other._capacity);
				CopyFunctor func = _copyFunctors[static_cast<int>(_type)];
				assert(func != nullptr);
				(this->*func)(other._data, other._size);
			}
			_size = other._size;
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<int> list) 
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.i + _size++)int(value);
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<float> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.f + _size++)float(value);
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<vec4> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.v + _size++)vec4(value);
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<mat4> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.m + _size++)mat4(value);
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<string> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.s + _size++)string(value);
		}
		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<RTTI*> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.p + _size++)RTTI* (value);
		}
		return *this;
	}

	Datum& Datum::operator=(const int& value)
	{
		SetType(DatumType::Integer);
		if (_size != 1_z) Resize(1_z);
		*_data.i = value;
		return *this;
	}

	Datum& Datum::operator=(const float& value)
	{
		SetType(DatumType::Float);
		if (_size != 1_z) Resize(1_z);
		*_data.f = value;
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		SetType(DatumType::Vector);
		if (_size != 1_z) Resize(1_z);
		*_data.v = value;
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		SetType(DatumType::Matrix);
		if (_size != 1_z) Resize(1_z);
		*_data.m = value;
		return *this;
	}

	Datum& Datum::operator=(Scope& value)
	{
		SetType(DatumType::Table);
		if (_size != 1_z) Resize(1_z);
		*_data.t = &value;
		return *this;
	}

	Datum& Datum::operator=(const std::string& value)
	{
		SetType(DatumType::String);
		if (_size != 1_z) Resize(1_z);
		*_data.s = value;
		return *this;
	}

	Datum& Datum::operator=(RTTI* const& value)
	{
		SetType(DatumType::Pointer);
		if (_size != 1_z) Resize(1_z);
		*_data.p = value;
		return *this;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this != &other)
		{
			Datum::~Datum();
			_type = other._type;
			_data = other._data;
			_size = other._size;
			_capacity = other._capacity;

			other._type = DatumType::Unknown;
			other._data.vp = nullptr;
			other._size = 0_z;
			other._capacity = 0_z;
		}
		return *this;
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			Clear();
			free(_data.vp);
		}
	}

	Datum::DatumType Datum::Type() const
	{
		return _type;
	}

	void Datum::SetType(DatumType type)
	{
		if (type == DatumType::Unknown)
		{
			throw runtime_error("You cannot set to Unknown Type.");
		}
		if (_type != DatumType::Unknown && type != _type)
		{
			throw runtime_error("You cannot change a datum's type once set.");
		}

		_type = type;
	}

	size_t Datum::Size() const
	{
		return _size;
	}
	size_t Datum::Capacity() const
	{
		return _capacity;
	}

	bool Datum::IsExternal() const
	{
		return _isExternal;
	}

	bool Datum::IsEmpty() const
	{
		return _isExternal == false && _size == 0;
	}

	void Datum::Reserve(size_t capacity)
	{
		if (_isExternal)
		{
			throw runtime_error("Cannot Reserve with external storage.");
		}
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot Reserve with an unknown type.");
		}

		if (_capacity < capacity)
		{
			size_t size = _sizeMap[static_cast<int>(_type)];
			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}
	void Datum::Resize(size_t size)
	{
		if (_isExternal)
		{
			throw runtime_error("Cannot Resize with external storage.");
		}
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot Resize with an unknown type.");
		}

		if (size < _size)
		{
			if (_type == DatumType::String)
			{
				for (size_t i = size; i < _size; ++i)
				{
					_data.s[i].~string();
				}
			}
			_capacity = size;
		}
		else
		{
			Reserve(size);

			CreateFunctor func = _createFunctors[static_cast<int>(_type)];
			assert(func != nullptr);
			(this->*func)(_size, size);
		}
		_size = size;
	}

	void Datum::Clear()
	{
		if (_isExternal)
		{
			throw runtime_error("Cannot Clear with external storage.");
		}
		if (_type == DatumType::String)
		{
			for (size_t i = 0_z; i < _size; ++i)
			{
				_data.s[i].~string();
			}
		}

		_size = 0_z;
	}

	void Datum::SetStorage(int* data, size_t size)
	{
		SetStorage(DatumType::Integer, data, size);
	}

	void Datum::SetStorage(float* data, size_t size)
	{
		SetStorage(DatumType::Float, data, size);
	}

	void Datum::SetStorage(glm::vec4* data, size_t size)
	{
		SetStorage(DatumType::Vector, data, size);
	}

	void Datum::SetStorage(glm::mat4* data, size_t size)
	{
		SetStorage(DatumType::Matrix, data, size);
	}

	void Datum::SetStorage(std::string* data, size_t size)
	{
		SetStorage(DatumType::String, data, size);
	}

	void Datum::SetStorage(RTTI** data, size_t size)
	{
		SetStorage(DatumType::Pointer, data, size);
	}

	void Datum::SetStorage(DatumType type, void* data, size_t size)
	{
		if (size == 0)
		{
			throw runtime_error("Cannot SetStorage of size 0.");
		}
		if (_type == DatumType::Unknown)
		{
			SetType(type);
		}
		else if(_type != type)
		{
			throw runtime_error("Cannot SetStorage on different types.");
		}
		if (!_isExternal && !IsEmpty())
		{
			throw runtime_error("Cannot SetStorage on already initialized memory.");
		}
		_data.vp = data;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

#pragma region Comparators
	bool Datum::operator==(const Datum& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const Datum& other) const
	{
		if (_type == DatumType::Unknown || other._type == DatumType::Unknown)
		{
			if (_type == DatumType::Unknown && other._type == DatumType::Unknown) return false;
			throw runtime_error("Cannot compare with an unknown type.");
		}

		CompareFunctor func = _compareFunctors[static_cast<int>(_type)];
		assert(func != nullptr);

		return _size != other._size || _type != other._type || !(this->*func)(other);
	}

	bool Datum::operator==(const int& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const int& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		return _size != 1_z || _type != DatumType::Integer || *_data.i != other;
	}

	bool Datum::operator==(const float& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const float& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		return _size != 1_z || _type != DatumType::Float || *_data.f != other;
	}

	bool Datum::operator==(const vec4& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const vec4& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		return _size != 1_z || _type != DatumType::Vector || *_data.v != other;
	}

	bool Datum::operator==(const mat4& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const mat4& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		return _size != 1_z || _type != DatumType::Matrix || *_data.m != other;
	}

	bool Datum::operator==(const string& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const string& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		return _size != 1_z || _type != DatumType::String || (_data.s->compare(other) != 0);
	}

	bool Datum::operator==(RTTI* const& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(RTTI* const& other) const
	{
		if (_type == DatumType::Unknown)
		{
			throw runtime_error("Cannot compare with an unknown type.");
		}

		RTTI* lhsPointer = *_data.p;

		return _size != 1_z || _type != DatumType::Pointer || (lhsPointer != other && lhsPointer != nullptr && !lhsPointer->Equals(other));
	}

	Scope& Datum::operator[](size_t index)
	{
		return GetScope(index);
	}

	const Scope& Datum::operator[](size_t index) const
	{
		return GetScope(index);
	}

#pragma endregion

	void Datum::Set(const int& value, size_t index)
	{
		SetHelper(DatumType::Integer, index);
		_data.i[index] = value;
	}

	void Datum::Set(const float& value, size_t index)
	{
		SetHelper(DatumType::Float, index);
		_data.f[index] = value;
	}

	void Datum::Set(const vec4& value, size_t index)
	{
		SetHelper(DatumType::Vector, index);
		_data.v[index] = value;
	}

	void Datum::Set(const mat4& value, size_t index)
	{
		SetHelper(DatumType::Matrix, index);
		_data.m[index] = value;
	}

	void Datum::Set(Scope& value, size_t index)
	{
		SetHelper(DatumType::Table, index);
		_data.t[index] = &value;
	}

	void Datum::Set(const string& value, size_t index)
	{
		SetHelper(DatumType::String, index);
		_data.s[index] = value;
	}

	void Datum::Set(RTTI* const& value, size_t index)
	{
		SetHelper(DatumType::Pointer, index);
		_data.p[index] = value;
	}

	void Datum::SetHelper(DatumType type, size_t index)
	{
		if (_type != type)
		{
			throw runtime_error("Cannot set on a different type, is the type Unknown?");
		}
		if (index >= _size)
		{
			throw out_of_range("Index out of range, is index the length of the Datum?");
		}
	}

	int& Datum::GetInteger(size_t index)
	{
		GetHelper(DatumType::Integer, index);
		return _data.i[index];
	}

	float& Datum::GetFloat(size_t index)
	{
		GetHelper(DatumType::Float, index);
		return _data.f[index];
	}

	vec4& Datum::GetVector(size_t index)
	{
		GetHelper(DatumType::Vector, index);
		return _data.v[index];
	}

	mat4& Datum::GetMatrix(size_t index)
	{
		GetHelper(DatumType::Matrix, index);
		return _data.m[index];
	}

	Scope& Datum::GetScope(size_t index)
	{
		GetHelper(DatumType::Table, index);
		return *_data.t[index];
	}

	string& Datum::GetString(size_t index)
	{
		GetHelper(DatumType::String, index);
		return _data.s[index];
	}

	RTTI*& Datum::GetPointer(size_t index)
	{
		GetHelper(DatumType::Pointer, index);
		return _data.p[index];
	}


	const int& Datum::GetInteger(size_t index) const
	{
		GetHelper(DatumType::Integer, index);
		return _data.i[index];
	}

	const float& Datum::GetFloat(size_t index) const
	{
		GetHelper(DatumType::Float, index);
		return _data.f[index];
	}

	const vec4& Datum::GetVector(size_t index) const
	{
		GetHelper(DatumType::Vector, index);
		return _data.v[index];
	}

	const mat4& Datum::GetMatrix(size_t index) const
	{
		GetHelper(DatumType::Matrix, index);
		return _data.m[index];
	}

	const Scope& Datum::GetScope(size_t index) const
	{
		GetHelper(DatumType::Table, index);
		return *_data.t[index];
	}

	const string& Datum::GetString(size_t index) const
	{
		GetHelper(DatumType::String, index);
		return _data.s[index];
	}

	RTTI* const& Datum::GetPointer(size_t index) const
	{
		GetHelper(DatumType::Pointer, index);
		return _data.p[index];
	}

	void Datum::GetHelper(DatumType type, size_t index) const
	{
		if (_type != type)
		{
			throw std::runtime_error("Cannot Get with an unknown type or a different type.");
		}
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range, is index the length of the Datum?");
		}
	}

	const int Datum::FromStringToInt(const std::string& str) const
	{
		int i;
		sscanf_s(str.c_str(), "%i", &i);
		return i;
	}

	const float Datum::FromStringToFloat(const std::string& str) const
	{
		float f;
		sscanf_s(str.c_str(), "%f", &f);
		return f;
	}

	const glm::vec4 Datum::FromStringToVector(const std::string& str) const
	{
		glm::vec4 v;
		sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &v[0], &v[1], &v[2], &v[3]);
		return v;
	}

	const glm::mat4 Datum::FromStringToMatrix(const std::string& str) const
	{
		glm::mat4 m;
		sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&(m[0][0]), &(m[0][1]), &(m[0][2]), &(m[0][3]), &(m[1][0]), &(m[1][1]), &(m[1][2]), &(m[1][3]),
			&(m[2][0]), &(m[2][1]), &(m[2][2]), &(m[2][3]), &(m[3][0]), &(m[3][1]), &(m[3][2]), &(m[3][3]));
		return m;
	}

	void Datum::SetFromString(const std::string& str, size_t index)
	{
		if (_type == DatumType::Unknown || _type == DatumType::Pointer)
		{
			throw std::runtime_error("Cannot SetFromString with an unknown or pointer type");
		}
		SetFromStringFunctor func = _setFromStringFunctors[static_cast<int>(_type)];
		assert(func != nullptr);
		(this->*func)(str, index);
	}

	std::string Datum::ToString(size_t index) const
	{
		if (_type == DatumType::Unknown)
		{
			throw std::runtime_error("Cannot ToString with an unknown type");
		}
		ToStringFunctor func = _toStringFunctors[static_cast<int>(_type)];
		assert(func != nullptr);
		return (this->*func)(index);
	}

	void Datum::PopBack()
	{
		if (_isExternal)
		{
			throw runtime_error("Cannot PopBack with external storage.");
		}
		if (_type == DatumType::Unknown)
		{
			throw std::runtime_error("Cannot PopBack on an unknown datum type.");
		}
		if (!IsEmpty())
		{
			--_size;
			if (_type == DatumType::String)
			{
				_data.s[_size].~string();
			}
		}
	}

	int& Datum::FrontInteger()
	{
		FrontHelper(DatumType::Integer);
		return _data.i[0_z];
	}

	float& Datum::FrontFloat()
	{
		FrontHelper(DatumType::Float);
		return _data.f[0_z];
	}

	vec4& Datum::FrontVector()
	{
		FrontHelper(DatumType::Vector);
		return _data.v[0_z];
	}

	mat4& Datum::FrontMatrix()
	{
		FrontHelper(DatumType::Matrix);
		return _data.m[0_z];
	}

	string& Datum::FrontString()
	{
		FrontHelper(DatumType::String);
		return _data.s[0_z];
	}

	RTTI*& Datum::FrontPointer()
	{
		FrontHelper(DatumType::Pointer);
		return _data.p[0_z];
	}

	const int& Datum::FrontInteger() const
	{
		FrontHelper(DatumType::Integer);
		return _data.i[0_z];
	}

	const float& Datum::FrontFloat() const
	{
		FrontHelper(DatumType::Float);
		return _data.f[0_z];
	}

	const vec4& Datum::FrontVector() const
	{
		FrontHelper(DatumType::Vector);
		return _data.v[0_z];
	}

	const mat4& Datum::FrontMatrix() const
	{
		FrontHelper(DatumType::Matrix);
		return _data.m[0_z];
	}

	const string& Datum::FrontString() const
	{
		FrontHelper(DatumType::String);
		return _data.s[0_z];
	}

	RTTI* const& Datum::FrontPointer() const
	{
		FrontHelper(DatumType::Pointer);
		return _data.p[0_z];
	}

	void Datum::FrontHelper(DatumType type) const
	{
		if (_type != type)
		{
			throw std::runtime_error("Cannot get Front on an unknown datum type or a different type than already set.");
		}
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
	}

	int& Datum::BackInteger()
	{
		BackHelper(DatumType::Integer);
		return _data.i[_size - 1_z];
	}

	float& Datum::BackFloat()
	{
		BackHelper(DatumType::Float);
		return _data.f[_size - 1_z];
	}

	vec4& Datum::BackVector()
	{
		BackHelper(DatumType::Vector);
		return _data.v[_size - 1_z];
	}

	mat4& Datum::BackMatrix()
	{
		BackHelper(DatumType::Matrix);
		return _data.m[_size - 1_z];
	}

	string& Datum::BackString()
	{
		BackHelper(DatumType::String);
		return _data.s[_size - 1_z];
	}

	RTTI*& Datum::BackPointer()
	{
		BackHelper(DatumType::Pointer);
		return _data.p[_size - 1_z];
	}

	const int& Datum::BackInteger() const
	{
		BackHelper(DatumType::Integer);
		return _data.i[_size - 1_z];
	}

	const float& Datum::BackFloat() const
	{
		BackHelper(DatumType::Float);
		return _data.f[_size - 1_z];
	}

	const vec4& Datum::BackVector() const
	{
		BackHelper(DatumType::Vector);
		return _data.v[_size - 1_z];
	}

	const mat4& Datum::BackMatrix() const
	{
		BackHelper(DatumType::Matrix);
		return _data.m[_size - 1_z];
	}

	const string& Datum::BackString() const
	{
		BackHelper(DatumType::String);
		return _data.s[_size - 1_z];
	}

	RTTI* const& Datum::BackPointer() const
	{
		BackHelper(DatumType::Pointer);
		return _data.p[_size - 1_z];
	}

	void Datum::BackHelper(DatumType type) const
	{
		if (_type != type)
		{
			throw std::runtime_error("Cannot get Back on an unknown datum type or a different type than already set.");
		}
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
	}

	void Datum::RemoveAt(size_t index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot remove from a Datum with external storage");
		}

		if (index >= _size)
		{
			throw out_of_range("Index out of range.");
		}

		if (_type == DatumType::String)
		{
			_data.s[index].~basic_string();
		}

		const size_t elementCount = _size - index - 1;
		const auto elementSize = _sizeMap[static_cast<size_t>(_type)];
		const auto size = elementCount * elementSize;
		if (size > 0)
		{
			uint8_t* destination = reinterpret_cast<uint8_t*>(_data.vp) + index * elementSize;
			uint8_t* source = destination + elementSize;
			memmove_s(destination, size, source, size);
		}
		--_size;
	}

}