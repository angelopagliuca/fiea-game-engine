#include "pch.h"
#include "SList.h"
#include <stdexcept>

namespace FieaGameEngine
{
#pragma region Node
	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) :
		_data(data), _next(next)
	{
	}
#pragma endregion

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return (_owner != other._owner) || (_node != other._node);
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_node != nullptr)
		{
			_node = _node->_next;
		}
		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this Iterator == end()");
		}
		return _node->_data;
	}

	template<typename T>
	inline T* SList<T>::Iterator::operator->() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this Iterator == end()");
		}
		return &(_node->_data);
	}
#pragma endregion

#pragma region ConstIterator
	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _node(other._node)
	{
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_owner != other._owner) || (_node != other._node);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_node != nullptr)
		{
			_node = _node->_next;
		}
		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this Iterator == end()");
		}
		return _node->_data;
	}

	template<typename T>
	inline const T* SList<T>::ConstIterator::operator->() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this Iterator == end()");
		}
		return &(_node->_data);
	}
#pragma endregion

#pragma region SList
	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* next) :
		_data(std::forward<T>(data)), _next(next)
	{
	}

	template<typename T>
	inline SList<T>::SList(const SList& other)
	{
		for (const T& value : other)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& other) noexcept :
		_front(other._front), _back(other._back), _size(other._size)
	{
		other._front = nullptr;
		other._back = nullptr;
		other._size = 0_z;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();
			for (const T& value : other)
			{
				PushBack(value);
			}
		}
		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			_front = other._front;
			_back = other._back;
			_size = other._size;

			other._front = nullptr;
			other._back = nullptr;
			other._size = 0_z;
		}
		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		_front = new Node(value, _front);
		if (IsEmpty())
		{
			_back = _front;	
		}
		++_size;
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(T&& value)
	{
		_front = new Node(std::forward<T>(value), _front);
		if (_size == 0_z)
		{
			_back = _front;
		}
		++_size;
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{
		Node* newNode = new Node(value, nullptr);
		if (IsEmpty())
		{
			_front = newNode;
		}
		else
		{
			_back->_next = newNode;
		}
		_back = newNode;
		++_size;
		return Iterator(*this, newNode);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(T&& value)
	{
		Node* newNode = new Node(std::forward<T>(value), nullptr);
		if (IsEmpty())
		{
			_front = newNode;
		}
		else
		{
			_back->_next = newNode;
		}
		_back = newNode;
		++_size;
		return Iterator(*this, newNode);
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (!IsEmpty()) {
			Node* nodeToDelete = _front;
			_front = _front->_next;
			delete nodeToDelete;
			--_size;
		}
	}

	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (!IsEmpty())
		{
			Node* previousNode = _front;
			Node* currentNode = _front;
			while (currentNode != _back) 
			{
				previousNode = currentNode;
				currentNode = currentNode->_next;
			}
			previousNode->_next = nullptr;
			_back = previousNode;
			delete currentNode;
			--_size;
		}
		if (IsEmpty()) _front = nullptr;
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		Node* currentNode = _front;
		while (currentNode != nullptr)
		{
			Node* nodeToDelete = currentNode;
			currentNode = currentNode->_next;
			delete nodeToDelete;
		}

		_size = 0;
		_front = _back = nullptr;
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty list.");
		}
		return _front->_data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty list.");
		}
		return _front->_data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty list.");
		}
		return _back->_data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty()) {
			throw std::runtime_error("Can't return data of an empty list.");
		}
		return _back->_data;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0_z;
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& value)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator is either uninitialized or the iterator is not owned by this SList.");
		}

		Iterator itToReturn;

		if (it._node != nullptr)
		{
			Node* newNode = new Node(value, it._node->_next);
			it._node->_next = newNode;
			if (_back == it._node) _back = newNode;
			++_size;
			itToReturn = Iterator(*this, newNode);
		}
		else {
			itToReturn = PushBack(value);
		}
		return itToReturn;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, T&& value)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator is either uninitialized or the iterator is not owned by this SList.");
		}

		Iterator itToReturn;

		if (it._node != nullptr)
		{
			Node* newNode = new Node(std::forward<T>(value), it._node->_next);
			it._node->_next = newNode;
			if (_back == it._node) _back = newNode;
			++_size;
			itToReturn = Iterator(*this, newNode);
		}
		else {
			itToReturn = PushBack(value);
		}
		return itToReturn;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value, EqualityFunctor equalityFunctor)
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
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value, EqualityFunctor equalityFunctor) const
	{
		return const_cast<SList*>(this)->Find(value, equalityFunctor);
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool SList<T>::Remove(const T& value, EqualityFunctor equalityFunctor)
	{
		return Remove(Find(value, equalityFunctor));
	}

	template<typename T>
	inline bool SList<T>::Remove(const Iterator& it)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator is either uninitialized or the iterator is not owned by this SList.");
		}

		bool wasRemoved = false;

		if (it != end())
		{
			if (it._node == _back)
			{
				PopBack();
			}
			else
			{
				Node* nodeToDelete = it._node->_next;

				it._node->_data.~T();
				new (&it._node->_data)T(std::move(nodeToDelete->_data));
				it._node->_next = nodeToDelete->_next;
				delete nodeToDelete;

				if (it._node->_next == nullptr)
				{
					_back = it._node;
				}

				--_size;
			}

			wasRemoved = true;
		}

		return wasRemoved;
	}

#pragma endregion
}
