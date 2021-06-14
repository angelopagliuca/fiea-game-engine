#include "pch.h"

#include "HashMap.h"


namespace FieaGameEngine
{
#pragma region Iterator
	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::Iterator::Iterator(HashMap& owner, size_t index, ChainIteratorType chainIt) :
		_owner(&owner), _index(index), _chainIt(chainIt)
	{
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return *_chainIt;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::Iterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return &(*_chainIt);
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator!=(const Iterator& other) const
	{
		return (_owner != other._owner) || (_index != other._index) || (_chainIt != other._chainIt);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator& HashMap<TKey, TValue>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_index < _owner->BucketSize())
		{
			++_chainIt;
			if (_chainIt == _owner->_buckets[_index].end())
			{
				while(++_index < _owner->BucketSize())
				{
					if (!_owner->_buckets[_index].IsEmpty())
					{
						_chainIt = _owner->_buckets[_index].begin();
						break;
					};
				}
				if (_index == _owner->BucketSize()) _chainIt = ChainIteratorType{};
			}
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

#pragma endregion

#pragma region ConstIterator
	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index), _chainIt(other._chainIt)
	{
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::ConstIterator::ConstIterator(const HashMap& owner, size_t index, ConstChainIteratorType chainIt) :
		_owner(&owner), _index(index), _chainIt(chainIt)
	{
	}

	template<typename TKey, typename TValue>
	inline const typename HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return *_chainIt;
	}

	template<typename TKey, typename TValue>
	inline const typename HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::ConstIterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Null reference. Is iterator uninitialized?");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::out_of_range("Index out of range. Does this Iterator == end() or is the index >= size of container?");
		}
		return &(*_chainIt);
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_owner != other._owner) || (_index != other._index) || (_chainIt != other._chainIt);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator& HashMap<TKey, TValue>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated Iterator.");
		}
		if (_index < _owner->BucketSize())
		{
			++_chainIt;
			if (_chainIt == _owner->_buckets[_index].end())
			{
				while (++_index < _owner->BucketSize())
				{
					if (!_owner->_buckets[_index].IsEmpty())
					{
						_chainIt = _owner->_buckets[_index].begin();
						break;
					};
				}
				if (_index == _owner->BucketSize()) _chainIt = ConstChainIteratorType{};
			}
		}
		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}
#pragma endregion

#pragma region HashMap
	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(size_t size, HashFunctor hashFunctor, EqualityFunctor equalityFunctor) :
		_hashFunctor { hashFunctor }, _equalityFunctor{ equalityFunctor }
	{
		if (size == 0)
		{
			throw std::runtime_error("HashMap can NOT be initialized with a size of ZERO.");
		}

		_buckets.Resize(size);
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<PairType> list) :
		HashMap{ list, DefaultHash<TKey>(), DefaultEquality<TKey>() }
	{
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<PairType> list, HashFunctor hashFunctor, EqualityFunctor equalityFunctor) :
		HashMap{ list.size(), hashFunctor, equalityFunctor }
	{
		for (const auto& pair : list)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(HashMap&& other) noexcept :
		_buckets{ std::move(other._buckets) }, _size{ std::move(other._size) },
		_hashFunctor{ std::move(other._hashFunctor) }, _equalityFunctor{ std::move(other._equalityFunctor) }
	{
		other._size = 0_z;
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>& HashMap<TKey, TValue>::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			_buckets = std::move(other._buckets);
			_size = std::move(other._size);
			_hashFunctor = std::move(other._hashFunctor);
			_equalityFunctor = std::move(other._equalityFunctor);

			other._size = 0_z;
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::IsEmpty() const
	{
		return _size == 0_z;
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::Size() const
	{
		return _size;
	}
	
	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::BucketSize() const
	{
		return _buckets.Size();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Find(const TKey& key)
	{
		size_t index = _hashFunctor(key) % BucketSize();
		ChainType& bucket = _buckets[index];

		ChainIteratorType it = bucket.begin();
		for (; it != bucket.end(); ++it)
		{
			if (_equalityFunctor(it->first, key))
			{
				break;
			}
		}
		if (it == bucket.end())
		{
			index = BucketSize();
			it = ChainIteratorType{};
		}
		return Iterator(*this, index, it);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::Find(const TKey& key) const
	{
		size_t index = _hashFunctor(key) % BucketSize();
		const ChainType& bucket = _buckets[index];

		ConstChainIteratorType it = bucket.begin();
		for (; it != bucket.end(); ++it)
		{
			if (_equalityFunctor(it->first, key))
			{
				break;
			}
		}
		if (it == bucket.end())
		{
			index = BucketSize();
			it = ConstChainIteratorType{};
		}
		return ConstIterator(*this, index, it);
	}

	template<typename TKey, typename TValue>
	inline std::pair<typename HashMap<TKey, TValue>::Iterator, bool> HashMap<TKey, TValue>::Insert(const PairType& entry)
	{
		bool wasValueInserted = false;
		size_t index = _hashFunctor(entry.first) % BucketSize();
		ChainType& bucket = _buckets[index];

		ChainIteratorType it = bucket.begin();
		for (; it != bucket.end(); ++it)
		{
			if (_equalityFunctor(it->first, entry.first))
			{
				break;
			}
		}

		if (it == bucket.end())
		{
			it = bucket.PushFront(entry);
			wasValueInserted = true;
			++_size;
		}
		
		return std::make_pair(Iterator(*this, index, it), wasValueInserted);
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Remove(const TKey& key)
	{
		Iterator it = Find(key);
		if (it != end())
		{
			_buckets[it._index].Remove(it._chainIt);
			--_size;
		}
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Clear()
	{
		for (ChainType& bucket : _buckets)
		{
			bucket.Clear();
		}

		_size = 0_z;
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Resize(size_t bucketSize)
	{
		HashMap map{ bucketSize, _hashFunctor, _equalityFunctor };

		for (auto& it : *this)
		{
			map.Insert(std::move(it));
		}

		*this = std::move(map);
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::operator[](const TKey& key)
	{
		auto [it, wasInserted] = Insert(std::make_pair(key, TValue()));
		return it->second;
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::At(const TKey& key)
	{
		Iterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("No Value associated with key passed in to HashMap.At()");
		}
		return it->second;
	}

	template<typename TKey, typename TValue>
	inline const TValue& HashMap<TKey, TValue>::At(const TKey& key) const
	{
		ConstIterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("No Value associated with key passed in to HashMap.At()");
		}
		return it->second;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin()
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			ChainType& bucket = _buckets[i];
			if (!bucket.IsEmpty())
			{
				return Iterator(*this, i, bucket.begin());
			}
		}
		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			const ChainType& bucket = _buckets[i];
			if (!bucket.IsEmpty())
			{
				return ConstIterator(*this, i, bucket.begin());
			}
		}
		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cbegin() const
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			const ChainType& bucket = _buckets[i];
			if (!bucket.IsEmpty())
			{
				return ConstIterator(*this, i, bucket.begin());
			}
		}
		return cend();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end()
	{
		return Iterator(*this, BucketSize(), ChainIteratorType{});
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const
	{
		return ConstIterator(*this, BucketSize(), ConstChainIteratorType{});
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cend() const
	{
		return ConstIterator(*this, BucketSize(), ConstChainIteratorType{});
	}
#pragma endregion
}
