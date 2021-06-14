#include "pch.h"

#include "Vector.h"
#include <cassert>

namespace FieaGameEngine
{
#pragma region Iterator
	template<typename T>
	inline Vector<T>::Iterator::Iterator(Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		return _owner->At(_index);
	}
	template<typename T>
	inline T* Vector<T>::Iterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		return &(_owner->At(_index));
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return (_owner != other._owner) || (_index != other._index);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator<(const Iterator& other) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_owner != other._owner)
		{
			throw std::runtime_error("Unrelated Iterators.");
		}
		return _index < other._index;
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator>(const Iterator& other) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_owner != other._owner)
		{
			throw std::runtime_error("Unrelated Iterators.");
		}
		return _index > other._index;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_index <= _owner->_size)
		{
			++_index;
		}
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		_index = std::max(0_z, _index - 1_z);
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp(*this);
		operator--();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Uninitialized iterator.");
		}
		_index += offset;
		return *this;
	}
	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Uninitialized iterator.");
		}
		_index = std::max(0_z, _index - offset);
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator+(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return Iterator(*_owner, _index + offset);
	}
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator-(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return Iterator(*_owner, std::max(0_z, _index - offset));
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator[](std::size_t index) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return _owner->At(_index + index);
	}
#pragma endregion

#pragma region ConstIterator
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		return _owner->At(_index);
	}

	template<typename T>
	inline const T* Vector<T>::ConstIterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		return &(_owner->At(_index));
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_owner != other._owner) || (_index != other._index);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator<(const ConstIterator& other) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_owner != other._owner)
		{
			throw std::runtime_error("Unrelated Iterators.");
		}
		return _index < other._index;
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator>(const ConstIterator& other) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_owner != other._owner)
		{
			throw std::runtime_error("Unrelated Iterators.");
		}
		return _index > other._index;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_index < _owner->_size)
		{
			++_index;
		}
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		_index = std::max(0_z, _index - 1_z);
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp(*this);
		operator--();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Uninitialized iterator.");
		}
		_index += offset;
		return *this;
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator-=(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Uninitialized iterator.");
		}
		_index = std::max(0_z, _index - offset);
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return ConstIterator(*_owner, _index + offset);
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(size_t offset)
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return ConstIterator(*_owner, std::max(0_z, _index - offset));
	}

	template<typename T>
	inline T& Vector<T>::ConstIterator::operator[](std::size_t index) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unitialized iterator.");
		}
		return _owner->At(_index + index);
	}

#pragma endregion

#pragma region Vector
	template<typename T>
	inline Vector<T>::Vector(size_t capacity)
	{
		if (capacity > 0_z)
		{
			Reserve(capacity);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list) :
		Vector(list.size())
	{
		for (const auto& value : list)
		{
			new(_data + _size++)T(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& other)
	{
		Reserve(other._capacity);
		for (const T& value : other)
		{
			new(_data + _size++)T(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept:
		_data(other._data), _size(other._size), _capacity(other._capacity)
	{
		other._data = nullptr;
		other._size = 0_z;
		other._capacity = 0_z;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Vector::~Vector();
			Reserve(other._capacity);
			for (const T& value : other)
			{
				new(_data + _size++)T(value);
			}
		}
		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Vector::~Vector();
			_data = other._data;
			_size = other._size;
			_capacity = other._capacity;

			other._data = nullptr;
			other._size = 0_z;
			other._capacity = 0_z;
		}
		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(std::initializer_list<T> list)
	{
		Clear();
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data + _size++)T(value);
		}
		return *this;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		for (size_t i = 0_z; i < _size; ++i)
		{
			_data[i].~T();
		}
		free(_data);
	}

	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		return At(index);
	}

	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		return At(index);
	}

	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return _data[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return _data[index];
	}

	template<typename T>
	template<typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& value, IncrementFunctor incrementFunctor)
	{
		if (_size == _capacity)
		{
			size_t capacity = _capacity  + std::max(1_z, incrementFunctor(_size, _capacity));
			Reserve(capacity);
		}

		new(_data + _size)T(value);

		return Iterator(*this, _size++);
	}

	template<typename T>
	template<typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(T&& value, IncrementFunctor incrementFunctor)
	{
		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunctor(_size, _capacity));
			Reserve(capacity);
		}

		new(_data + _size)T(std::forward<T>(value));

		return Iterator(*this, _size++);
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			_data[--_size].~T();
		}
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
		return _data[0_z];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
		return _data[0_z];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
		return _data[_size-1_z];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty container.");
		}
		return _data[_size-1_z];
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0_z;
	}
	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return _size;
	}
	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0_z);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, _size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, _size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, _size);
	}

	template <typename T>
	inline void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > _capacity)
		{
			T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T)* capacity));
			assert(data != nullptr);
			_data = data;
			_capacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size)
	{
		if (size < _size)
		{
			for (size_t i = size; i < _size; ++i)
			{
				_data[i].~T();
			}
		}
		else
		{
			Reserve(size);
			for (size_t i = _size; i < size; ++i)
			{
				new (_data + i)T();
			}
		}
		_size = size;
	}

	template<typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0_z; i < _size; ++i)
		{
			_data[i].~T();
		}

		_size = 0_z;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (_capacity > _size)
		{
			if (_size == 0_z)
			{
				free(_data);
				_data = nullptr;
			}
			else
			{
				T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * _size));
				assert(data != nullptr);
				_data = data;
			}
			_capacity = _size;
		}
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value, EqualityFunctor equalityFunctor)
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<Vector*>(this)->Find(value, equalityFunctor);
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool Vector<T>::Remove(const T& value, EqualityFunctor equalityFunctor)
	{
		return Remove(Find(value, equalityFunctor));
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& it)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator is either uninitialized or the iterator is not owned by this SList.");
		}

		bool wasRemoved = false;

		if (it != end())
		{
			_data[it._index].~T();
			memmove(_data + it._index, _data + it._index + 1, sizeof(T) * (_size - it._index - 1));
			--_size;

			wasRemoved = true;
		}

		return wasRemoved;
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if (this != first._owner || this != last._owner)
		{
			throw std::runtime_error("Iterator is either uninitialized or the iterator is not owned by this SList.");
		}

		bool wasRemoved = false;

		if (first < last)
		{
			for (Iterator it = first; it != last; ++it)
			{
				_data[it._index].~T();
			}

			if (last != end()) memmove(_data + first._index, _data + last._index, sizeof(T) * (_size - last._index));
			_size -= last._index - first._index;

			wasRemoved = true;
		}
		return wasRemoved;
	}
#pragma endregion
}
