#pragma once

#include "DefaultEquality.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
	/// <summary>
	/// SList is a singled linked list class, it has a Node containing data
	/// where the nodes themselves have a 'link' to the next node.
	/// </summary>
	/// <typeparam name="T">Type of data to be stored in list (container)</typeparam>
	template <typename T>
	class SList 
	{
	private:
		struct Node final
		{
			/// <summary>
			/// Creates Node element with data and points to next element passed in, nullptr by default
			/// </summary>
			/// <param name="data">data to place in node</param>
			/// <param name="next">node to point to as next</param>
			Node(const T& data, Node* next = nullptr);
			/// <summary>
			/// Creates Node element with rvalue data and points to next element passed in, nullptr by default
			/// </summary>
			/// <param name="data">rvalue data to place in node</param>
			/// <param name="next">node to point to as next</param>
			Node(T&& data, Node* next = nullptr);

			T _data;
			Node* _next;
		};

	public:
		class Iterator final
		{
			friend SList;
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
			Iterator& operator=(const Iterator& other) = default;
			/// <summary>
			/// move operation for iterator using default move semantics provided by compiler
			/// </summary>
			/// <param name="other">other iterator to move</param>
			/// <returns>iterator address of the moved iterator</returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			/// <summary>
			/// Default Destructor of Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// !(operator!=(other))
			/// </summary>
			/// <param name="other"></param>
			/// <returns>!(operator!=(other))</returns>
			bool operator==(const Iterator& other) const;
			/// <summary>
			/// The iterators are not equal if the owners are not the same or the nodes are not the same
			/// </summary>
			/// <param name="other">other iterator to compare to</param>
			/// <returns>true if not equal and false otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// pre increments iterator by pointing to the node next to the current node its pointing at
			/// </summary>
			/// <returns>reference to the iterator</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator& operator++();
			/// <summary>
			/// increments iterator by pointing to the node next to the current node its pointing at after
			/// expression is over
			/// </summary>
			/// <returns>copy of iterator before adding to it</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator operator++(int);

			/// <summary>
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			T& operator*() const;
			/// <summary>
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			T* operator->() const;

		private:
			Iterator(const SList& owner, Node* node);
			const SList* _owner{ nullptr };
			Node* _node{ nullptr };
		};

		class ConstIterator final
		{
			friend SList;

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
			/// <returns>copy of constant iterator before adding to it</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			const T& operator*() const;
			/// <summary>
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			const T* operator->() const;

		private:
			ConstIterator(const SList& owner, Node* node);
			const SList* _owner{ nullptr };
			const Node* _node{ nullptr };
		};


		/// <summary>
		/// Default constructor 
		/// </summary>
		SList() = default;
		/// <summary>
		/// Initialized copy of linked lists
		/// </summary>
		/// <param name="other">the other list (rhs)</param>
		SList(const SList& other);
		/// <summary>
		/// Initialized moved linked list, will break the moved list
		/// </summary>
		/// <param name="other">the other list (rhs)</param>
		SList(SList&& other) noexcept;
		/// <summary>
		/// Copy assign operator
		/// </summary>
		/// <param name="other">the other list (rhs) to be copied</param>
		/// <returns>the copied list</returns>
		SList& operator=(const SList& other);
		/// <summary>
		/// Moved assigned operator
		/// </summary>
		/// <param name="other">the other list (rhs) to be moved</param>
		/// <returns>the moved list</returns>
		SList& operator=(SList&& other) noexcept;
		~SList();

		/// <summary>
		/// Pushes the data (lvalue) into the front of the linked list
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <returns>Iterator pointint to pushed element</returns>
		Iterator PushFront(const T& value);
		/// <summary>
		/// Pushes the data (rvalue) into the front of the linked list 
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <returns>Iterator pointint to pushed element</returns>

		Iterator PushFront(T&& value);
		/// <summary>
		/// Pushes the data (lvalue) into the back of the linked list 
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <returns>Iterator pointint to pushed element</returns>
		Iterator PushBack(const T& value);
		/// <summary>
		/// Pushes the data (rvalue) into the back of the linked list 
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <returns>Iterator pointint to pushed element</returns>
		Iterator PushBack(T&& value);

		/// <summary>
		/// deletes the first (front) element of the linked list, will do nothing if empty
		/// </summary>
		void PopFront();
		/// <summary>
		/// deletes the last (back) element of the linked list
		/// </summary>
		void PopBack();

		/// <summary>
		/// Completely clears the linked list of its data leaving it empty
		/// </summary>
		void Clear();

		/// <summary>
		/// returns the data of the front element as its type
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if list is empty</exception>
		T& Front();
		/// <summary>
		/// returns the data of the front element as its type (const linked list)
		/// </summary>
		/// <returns>returns the  element as constant at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if list is empty</exception>
		const T& Front() const;
		/// <summary>
		/// returns the data of the back element as its type
		/// </summary>
		/// <returns></returns>
		/// <exception cref="runtime_error">throws exception if list is empty</exception>
		T& Back();
		/// <summary>
		/// returns the data of the back element as its type (const linked list)
		/// </summary>
		/// <returns></returns>
		/// <exception cref="runtime_error">throws exception if list is empty</exception>
		const T& Back() const;

		/// <summary>
		/// Is list empty?
		/// </summary>
		/// <returns>true if list is empty false otherwise</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Size of the list
		/// </summary>
		/// <returns>the size of the container</returns>
		size_t Size() const;

		/// <summary>
		/// returns an SList::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>the iterator pointing to the first element of the container (front)</returns>
		Iterator begin();
		/// <summary>
		/// (const) returns an SList::ConstIterator pointing to the head of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the constant container (front)</returns>
		ConstIterator begin() const;
		/// <summary>
		/// returns an SList::ConstIterator pointing to the head of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the container (front)</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// returns an SList::Iterator pointing past the end of the list.
		/// </summary>
		/// <returns>the iterator pointing to past the last element of the container (back)</returns>
		Iterator end();
		/// <summary>
		/// (const) returns an SList::ConstIterator pointing past the end of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the constant container (back)</returns>
		ConstIterator end() const;
		/// <summary>
		/// returns an SList::ConstIterator pointint past the end of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the container (back)</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Appends the given item after the item the given iterator points to.
		/// </summary>
		/// <param name="it">iterator to insert after</param>
		/// <param name="value">value to insert</param>
		/// <returns>new iterator thats inserted after the given iterator</returns>
		/// <exception cref="runtime_error">throws exception if given iterator is not owned by list to insert value to</exception>
		Iterator InsertAfter(const Iterator& it, const T& value);
		/// <summary>
		/// Appends the given rvalue item after the item the given iterator points to.
		/// </summary>
		/// <param name="it">iterator to insert after</param>
		/// <param name="value">rvalue to insert</param>
		/// <returns>new iterator thats inserted after the given iterator</returns>
		/// <exception cref="runtime_error">throws exception if given iterator is not owned by list to insert rvalue to</exception>
		Iterator InsertAfter(const Iterator& it, T&& value);

		/// <summary>
		/// Finds and returns first iterator pointing to item passed in, if not found it returns end.
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in list</param>
		/// <returns>Iterator pointing to node containing value or end if not found</returns>
		template<typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns first iterator pointing to rvalue item passed in, if not found it returns end.
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">rvalue to find in list</param>
		/// <returns>Iterator pointing to node containing rvalue or end if not found</returns>
		template<typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;

		/// <summary>
		/// Finds and Removes element of the list containing the value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<T>>
		bool Remove(const T& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Removes the element pointed at by iterator passed in.
		/// </summary>
		/// <param name="value">iterator to remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		/// <exception cref="runtime_error">throws exception if given iterator is not owned by list to insert rvalue to</exception>
		bool Remove(const Iterator& it);

	private:
		Node* _front{ nullptr };
		Node* _back{ nullptr };
		size_t _size{ 0_z };

	};
}

#include "SList.inl"
