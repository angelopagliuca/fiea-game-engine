#pragma once

#include <utility>
#include <functional>

#include "DefaultHash.h"
#include "DefaultEquality.h"
#include "SizeLiteral.h"
#include "Vector.h"
#include "SList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// HashMap is an unordered map storing keys at indexes based on what unsigned int
	/// they get hashed to. HashFunctor can be passed in but must guarantee that equivalent
	/// keys will hash to the same result. EqualityFunctor is used to compared the keys and
	/// can be passed in as well. Size of the vector will be constant throughout.
	/// </summary>
	/// <typeparam name="TKey">Key that will be hashed to access map</typeparam>
	/// <typeparam name="TValue">Value to be stored at key</typeparam>
	template <typename TKey, typename TValue>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TValue>;
		using HashFunctor = std::function<size_t(const TKey& key)>;
		using EqualityFunctor = std::function<bool(const TKey& lhs, const TKey& rhs)>;
		using IncrementFunctor = std::function<size_t(size_t size, size_t capacity)>;

	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIteratorType = typename ChainType::Iterator;
		using ConstChainIteratorType = typename ChainType::ConstIterator;

	public:
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Constructs iterator using default constructor provided by compiler
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Copies iterator using default copy semantics provided by compiler
			/// </summary>
			/// <param name="">other iterator to copy</param>
			Iterator(const Iterator&) = default;
			/// <summary>
			/// Moves iterator using default move semantics provided by compiler
			/// </summary>
			/// <param name="">other iterator to move</param>
			Iterator(Iterator&&) noexcept = default;
			/// <summary>
			/// Copy operation for iterator using default copy semantics provided by compiler
			/// </summary>
			/// <param name="other">other iterator to copy</param>
			/// <returns>iterator address of the copied iterator</returns>
			Iterator& operator=(const Iterator & other) = default;
			/// <summary>
			/// move operation for iterator using default move semantics provided by compiler
			/// </summary>
			/// <param name="other">other iterator to move</param>
			/// <returns>iterator address of the moved iterator</returns>
			Iterator& operator=(Iterator && other) noexcept = default;
			/// <summary>
			/// Default Destructor of Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			PairType& operator*() const;
			/// <summary>
			/// returns pointer to the data that the iterator is pointing to.
			/// </summary>
			/// <returns>pointer to data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			PairType* operator->() const;

			/// <summary>
			/// !(operator!=(other))
			/// </summary>
			/// <param name="other"></param>
			/// <returns>!(operator!=(other))</returns>
			bool operator==(const Iterator & other) const;
			/// <summary>
			/// The iterators are not equal if the owners are not the same or the nodes are not the same
			/// </summary>
			/// <param name="other">other iterator to compare to</param>
			/// <returns>true if not equal and false otherwise</returns>
			bool operator!=(const Iterator & other) const;

			/// <summary>
			/// pre increments iterator by incrementing index
			/// </summary>
			/// <returns>reference to the iterator</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator& operator++();
			/// <summary>
			/// increments iterator by incrementing index
			/// expression is over
			/// </summary>
			/// <returns>copy of iterator before increment</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator operator++(int);

		private:
			Iterator(HashMap& owner, size_t index, ChainIteratorType chainIt);
			HashMap* _owner{ nullptr };
			size_t _index{ 0_z };
			ChainIteratorType _chainIt{ ChainIteratorType{} };

		};

		class ConstIterator final
		{
			friend HashMap;

		public:
			/// <summary>
			/// Constructs constant iterator using default constructor provided by compiler
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Casts non constant iterator into constant iterator
			/// </summary>
			/// <param name="it">iterator to cast</param>
			ConstIterator(const Iterator& it);
			/// <summary>
			/// Copies constant iterator using default copy semantics provided by compiler
			/// </summary>
			/// <param name="">other constant iterator to copy</param>
			ConstIterator(const ConstIterator&) = default;
			/// <summary>
			/// Moves constant iterator using default move semantics provided by compiler
			/// </summary>
			/// <param name="">other constant iterator to move</param>
			ConstIterator(ConstIterator&&) noexcept = default;
			/// <summary>
			/// Copy operation for constant iterator using default copy semantics provided by compiler
			/// </summary>
			/// <param name="other">other constant iterator to copy</param>
			/// <returns>iterator address of the copied constant iterator</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			/// <summary>
			/// move operation for constant iterator using default move semantics provided by compiler
			/// </summary>
			/// <param name="other">other constant iterator to move</param>
			/// <returns>iterator address of the moved constant iterator</returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Default Destructor of ConstIterator
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// returns const reference to data that the const iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereferenced</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			const PairType& operator*() const;
			/// <summary>
			/// returns const pointer to the data that the iterator is pointing to.
			/// </summary>
			/// <returns>pointer to data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			const PairType* operator->() const;

			/// <summary>
			/// !(operator!=(other))
			/// </summary>
			/// <param name="other"></param>
			/// <returns>!(operator!=(other))</returns>
			bool operator==(const ConstIterator& other) const;
			/// <summary>
			/// The constant iterators are not equal if the owners are not the same or the nodes are not the same
			/// </summary>
			/// <param name="other">other constant iterator to compare to</param>
			/// <returns>true if not equal and false otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// pre increments iterator by pointing to the node next to the current node its pointing at
			/// </summary>
			/// <returns>reference to the iterator</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			ConstIterator& operator++();
			/// <summary>
			/// increments iterator by pointing to the node next to the current node its pointing at after
			/// expression is over
			/// </summary>
			/// <returns>copy of iterator before increment</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			ConstIterator operator++(int);

		private:
			ConstIterator(const HashMap& owner, size_t index, ConstChainIteratorType chainIt);
			const HashMap* _owner{ nullptr };
			size_t _index{ 0_z };
			ConstChainIteratorType _chainIt{ ChainIteratorType{} };
		};

		/// <summary>
		/// Creates new instance of HashMap container. Requires a size > 0.
		/// Can pass in a custom HashFunctor as well as an EqualityFunctor, but
		/// both are required when one is passed in.
		/// </summary>
		/// <param name="size">Number of buckets</param>
		/// <param name="hashFuctor">function to use to hash keys</param>
		/// <param name="equalityFunctor">function to use to check for key equality</param>
		/// <exception cref="runtime-error">size cannot be zero</exception>
		HashMap(size_t size = 11_z, HashFunctor hashFunctor = DefaultHash<TKey>{}, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		/// <summary>
		/// Hash Map initializer list constructor with default functors
		/// </summary>
		/// <param name="list">initializer list</param>
		HashMap(std::initializer_list<PairType> list);
		/// <summary>
		/// Hash Map initializer list constructor with user defined functors
		/// </summary>
		/// <param name="list">initializer list</param>
		/// <param name="hashFunctor">hash functor</param>
		/// <param name="equalityFunctor">equality functor</param>
		HashMap(std::initializer_list<PairType> list, HashFunctor hashFunctor, EqualityFunctor equalityFunctor);
		/// <summary>
		/// Copy Constructor for new instance of HsshMap container.
		/// Deep copies other HashMap passed in.
		/// </summary>
		/// <param name="other">other vector to copy from</param>
		HashMap(const HashMap& other) = default;
		/// <summary>
		/// Move Constructor for new instance of HashMap container.
		/// Moves other HashMap passed in, making other HashMap invalid.
		/// Capacity will be the same capacity as other HashMap.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		HashMap(HashMap&& other) noexcept;
		/// <summary>
		/// Copy operator for HashMap container.
		/// Clears current HashMap and copies over the other HashMap.
		/// </summary>
		/// <param name="other">other HashMap to copy</param>
		/// <returns>copy of the other HashMap</returns>
		HashMap& operator=(const HashMap& other) = default;
		/// <summary>
		/// Move operator for HashMap container.
		/// Clears current HashMap and moves over the other HashMap.
		/// </summary>
		/// <param name="other">other HashMap to copy</param>
		/// <returns>moved HashMap</returns>
		HashMap& operator=(HashMap&& other) noexcept;
		/// <summary>
		/// Destructs HashMap container destructing elements and freeing data.
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Is HashMap empty?
		/// </summary>
		/// <returns>true if vector is empty false otherwise</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Size of the HashMap
		/// </summary>
		/// <returns>the amount of elements of the container</returns>
		size_t Size() const;
		/// <summary>
		/// Bucket size of the HashMap
		/// </summary>
		/// <returns>the size of the container</returns>
		size_t BucketSize() const;

		/// <summary>
		/// Finds and returns iterator pointing to chain where key resides, if not found it returns end.
		/// </summary>
		/// <param name="value">key to find in hashmap</param>
		/// <returns>Iterator pointing to index containing key or end if not found</returns>
		Iterator Find(const TKey& key);
		/// <summary>
		/// Finds and returns iterator pointing to chain where key resides, if not found it returns end.
		/// </summary>
		/// <param name="value">key to find in hashmap</param>
		/// <returns>ConstIterator pointing to index containing key or end if not found</returns>
		ConstIterator Find(const TKey& key) const;
		/// <summary>
		/// Insert will try to insert the entry passed in, if the entry is not in the hashmap
		/// then it will insert it and return the iterator pointing to the entry.
		/// If the entry is in the hashmap then it will not be inserted and the returned Iterator
		/// will be pointing to the already inserted entry.
		/// </summary>
		/// <param name="entry"></param>
		/// <returns>pair of Iterator where entry was inserted (if inserted) and bool if entry was inserted</returns>
		std::pair<Iterator, bool> Insert(const PairType& entry);

		/// <summary>
		/// Takes a “key” argument to remove and returns nothing.
		/// It removes the matching entry, if it exists, otherwise it does nothing.
		/// </summary>
		/// <param name="key">key entry to remove from hashmap</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Completely clears the hashmap of its data leaving all chains empty.
		/// </summary>
		void Clear();
		/// <summary>
		/// Resizes Map to a different Bucket Size.
		/// </summary>
		/// <param name="bucketSize">new size of the buckets</param>
		void Resize(size_t bucketSize);

		/// <summary>
		/// returns a Boolean indicating the presence of a key within the hash map.
		/// </summary>
		/// <param name="key">key to find</param>
		/// <returns>true if key is within the hash map, false otherwise</returns>
		bool ContainsKey(const TKey& key) const;
		/// <summary>
		/// Access to the value in the hashmap. If hashmap has no entry associated with
		/// the key then it will create a default entry. 
		/// </summary>
		/// <returns>reference to the item at the given index</returns>
		TValue& operator[](const TKey& key);
		/// <summary>
		/// Access to the data in the container [hashmap] at the given key. 
		/// </summary>
		/// <returns>reference to the value at the given index</returns>
		/// <exception cref="runtime_error">thrown if there is no key in container</exception>
		TValue& At(const TKey & key);
		/// <summary>
		/// access to the (constant) data in the container [hashmap] at the key. 
		/// </summary>
		/// <returns>constant reference to the value at the given key</returns>
		/// <exception cref="runtime_error">thrown if there is no key in container</exception>
		const TValue& At(const TKey & key) const;

		/// <summary>
		/// returns an HashMap::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>the iterator pointing to the first element of the container (front)</returns>
		Iterator begin();
		/// <summary>
		/// (const) returns an HashMap::ConstIterator pointing to the head of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the constant container (front)</returns>
		ConstIterator begin() const;
		/// <summary>
		/// returns an HashMap::ConstIterator pointing to the head of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the container (front)</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// returns an HashMap::Iterator pointing past the end of the list.
		/// </summary>
		/// <returns>the iterator pointing to past the last element of the container (back)</returns>
		Iterator end();
		/// <summary>
		/// (const) returns an HashMap::ConstIterator pointing past the end of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the constant container (back)</returns>
		ConstIterator end() const;
		/// <summary>
		/// returns an HashMap::ConstIterator pointint past the end of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the container (back)</returns>
		ConstIterator cend() const;

	private:
		BucketType _buckets;
		size_t _size{ 0_z };
		HashFunctor _hashFunctor;
		EqualityFunctor _equalityFunctor;
	};
}

#include "HashMap.inl"