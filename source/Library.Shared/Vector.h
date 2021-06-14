#pragma once

#include "DefaultIncrement.h"
#include "DefaultEquality.h"
#include "SizeLiteral.h"
#include <initializer_list>

namespace FieaGameEngine
{
	/// <summary>
	/// Vector Container. Contains any user provided type and is able to
	/// have random access into the container. User must provide increment
	/// functor in order to increment capacity as they'd like.
	/// </summary>
	/// <typeparam name="T">Type provided by user (Polymprphic Container)</typeparam>
	template <typename T>
	class Vector
	{
	public:
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
			using size_type = size_t;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

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
			/// returns data that the iterator is pointing to.
			/// </summary>
			/// <returns>data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			T& operator*() const;
			/// <summary>
			/// returns pointer to the data that the iterator is pointing to.
			/// </summary>
			/// <returns>pointer to data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			T* operator->() const;

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
			/// Less than operator for Vector Container's Iterator.
			/// </summary>
			/// <param name="other">iterator to compare to</param>
			/// <returns>if this iterator is less than the other return true otherwise false</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			/// <exception cref="runtime_error">unrelated iterators</exception>
			bool operator<(const Iterator& other) const;
			/// <summary>
			/// More than operator for Vector Container's Iterator.
			/// </summary>
			/// <param name="other">iterator to compare to</param>
			/// <returns>if this iterator is more than the other return true otherwise false</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			/// <exception cref="runtime_error">unrelated iterators</exception>
			bool operator>(const Iterator& other) const;

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
			/// <summary>
			/// pre decrements iterator by decrementing index
			/// </summary>
			/// <returns>reference to the iterator</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator& operator--();
			/// <summary>
			/// decrements iterator by decrementing index
			/// expression is over
			/// </summary>
			/// <returns>copy of iterator before increment</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			Iterator operator--(int);

			/// <summary>
			/// Plus equal operator for iterator. Same as operator+(it, offset)
			/// </summary>
			/// <param name="offset">amount to add</param>
			/// <returns>reference to added operator</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			Iterator& operator+=(size_t offset);
			/// <summary>
			/// Minus equal operator for iterator. Same as operator-(it, offset)
			/// </summary>
			/// <param name="offset">amount to substract</param>
			/// <returns>reference to substracted operator</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			Iterator& operator-=(size_t offset);

			/// <summary>
			/// Plus operator returning copy of iterator plus offset given.
			/// </summary>
			/// <param name="offset">offset to add to iterator</param>
			/// <returns>copy of iterator after operation</returns>
			Iterator operator+(size_t offset);
			/// <summary>
			/// Minus operator returning copy of iterator minus offset given.
			/// </summary>
			/// <param name="offset">offset to substract to iterator</param>
			/// <returns>copy of iterator after operation</returns>
			Iterator operator-(size_t offset);

			T& operator[](size_t index) const;

		private:
			Iterator(Vector& owner, size_t index);
			Vector* _owner{ nullptr };
			size_t _index{ 0_z };
		};

		class ConstIterator final
		{
			friend Vector; 


		public:
			using size_type = size_t;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

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
			const T& operator*() const;
			/// <summary>
			/// returns const pointer to the data that the iterator is pointing to.
			/// </summary>
			/// <returns>pointer to data pointed to by iterators</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			/// <exception cref="out_of_range">thrown if the index is invalid</exception>
			const T* operator->() const;

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
			/// Less than operator for Vector Container's Iterator.
			/// </summary>
			/// <param name="other">iterator to compare to</param>
			/// <returns>if this iterator is less than the other return true otherwise false</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			bool operator<(const ConstIterator& other) const;
			/// <summary>
			/// More than operator for Vector Container's Iterator.
			/// </summary>
			/// <param name="other">iterator to compare to</param>
			/// <returns>if this iterator is more than the other return true otherwise false</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			bool operator>(const ConstIterator& other) const;

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
			/// <summary>
			/// pre decrements constant iterator by decrementing index
			/// </summary>
			/// <returns>reference to the constant iterator</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			ConstIterator& operator--();
			/// <summary>
			/// decrements constant iterator by decrementing index
			/// expression is over
			/// </summary>
			/// <returns>copy of constant iterator before increment</returns>
			/// <exception cref="runtime_error">uninitialized iterator</exception>
			ConstIterator operator--(int);


			/// <summary>
			/// Plus equal operator for iterator. Same as operator+(it, offset)
			/// </summary>
			/// <param name="offset">amount to add</param>
			/// <returns>reference to added operator</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			ConstIterator& operator+=(size_t offset);
			/// <summary>
			/// Minus equal operator for iterator. Same as operator-(it, offset)
			/// </summary>
			/// <param name="offset">amount to substract</param>
			/// <returns>reference to substracted operator</returns>
			/// <exception cref="">nullptr not allowed to be dereference</exception>
			ConstIterator& operator-=(size_t offset);

			/// <summary>
			/// Plus operator returning copy of iterator plus offset given.
			/// </summary>
			/// <param name="offset">offset to add to iterator</param>
			/// <returns>copy of iterator after operation</returns>
			ConstIterator operator+(size_t offset);
			/// <summary>
			/// Minus operator returning copy of iterator minus offset given.
			/// </summary>
			/// <param name="offset">offset to substract to iterator</param>
			/// <returns>copy of iterator after operation</returns>
			ConstIterator operator-(size_t offset);

			T& operator[](size_t index) const;

		private:
			ConstIterator(const Vector& owner, size_t index);
			const Vector* _owner{ nullptr };
			size_t _index{ 0_z };
		};

		/// <summary>
		/// Creates new instance of vector container, if capacity is passed in it will
		/// automatically reserve the space needed in memory for type <T>
		/// </summary>
		/// <param name="capacity">Size to Reserve at initialization</param>
		Vector(size_t capacity = 0_z);
		/// <summary>
		/// Creates new instance of vector container using an initializer list.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Vector(std::initializer_list<T> list);
		/// <summary>
		/// Copy Constructor for new instance of vector container.
		/// Deep copies other vector passed in.
		/// Capacity will be the same capacity as other vector.
		/// </summary>
		/// <param name="other">other vector to copy from</param>
		Vector(const Vector& other);
		/// <summary>
		/// Move Constructor for new instance of vector container.
		/// Moves other vector passed in, making other vector invalid.
		/// Capacity will be the same capacity as other vector.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		Vector(Vector&& other) noexcept;
		/// <summary>
		/// Copy operator for vector container.
		/// Clears current vector and copies over the other vector.
		/// </summary>
		/// <param name="other">other vector to copy</param>
		/// <returns>copy of the other vector</returns>
		Vector& operator=(const Vector& other);
		/// <summary>
		/// Move operator for vector container.
		/// Clears current vector and moves over the other vector.
		/// </summary>
		/// <param name="other">other vector to copy</param>
		/// <returns>moved vector</returns>
		Vector& operator=(Vector&& other) noexcept;
		/// <summary>
		/// Copy operator for vector container using initializer list.
		/// Clears current vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as vector</returns>
		Vector& operator=(std::initializer_list<T> list);
		/// <summary>
		/// Destructs Vector container destructing elements and freeing data.
		/// </summary>
		~Vector();

		/// <summary>
		/// Access to the data in the container [vector]. 
		/// </summary>
		/// <returns>reference to the item at the given index</returns>
		/// <exception cref="out_of_range">thrown if the index is invalid</exception>
		T& operator[](size_t index);
		/// <summary>
		/// Constant access to the data in the container [vector]. 
		/// </summary>
		/// <returns>reference to the item at the given index</returns>
		/// <exception cref="out_of_range">thrown if the index is invalid</exception>
		const T& operator[](size_t index) const;
		/// <summary>
		/// Access to the data in the container [vector]. 
		/// </summary>
		/// <returns>reference to the item at the given index</returns>
		/// <exception cref="out_of_range">thrown if the index is invalid</exception>
		T& At(size_t index);
		/// <summary>
		/// Constant access to the data in the container [vector]. 
		/// </summary>
		/// <returns>reference to the item at the given index</returns>
		/// <exception cref="out_of_range">thrown if the index is invalid</exception>
		const T& At(size_t index) const;


		/// <summary>
		/// Pushes the data (lvalue) into the back of the vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <returns>Iterator pointing to pushed element</returns>
		template<typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(const T& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the data (rvalue) into the back of the vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <returns>Iterator pointing to pushed element</returns>
		template<typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(T&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// removes the last item in the container [vector], but does not reduce the capacity of the container.
		/// </summary>
		void PopBack();

		/// <summary>
		/// returns the data of the front element as its type
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if vector is empty</exception>
		T& Front();
		/// <summary>
		/// returns the data of the front element as its type (const linked list)
		/// </summary>
		/// <returns>returns the  element as constant at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if vector is empty</exception>
		const T& Front() const;
		/// <summary>
		/// returns the data of the back element as its type
		/// </summary>
		/// <returns></returns>
		/// <exception cref="runtime_error">throws exception if vector is empty</exception>
		T& Back();
		/// <summary>
		/// returns the data of the back element as its type (const linked list)
		/// </summary>
		/// <returns></returns>
		/// <exception cref="runtime_error">throws exception if vector is empty</exception>
		const T& Back() const;

		/// <summary>
		/// Is vector empty?
		/// </summary>
		/// <returns>true if vector is empty false otherwise</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Size of the vector
		/// </summary>
		/// <returns>the size of the container</returns>
		size_t Size() const;
		/// <summary>
		/// capicity of the vector
		/// </summary>
		/// <returns>the capacity of the container</returns>
		size_t Capacity() const;

		/// <summary>
		/// returns an Vector::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>the iterator pointing to the first element of the container (front)</returns>
		Iterator begin();
		/// <summary>
		/// (const) returns an Vector::ConstIterator pointing to the head of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the constant container (front)</returns>
		ConstIterator begin() const;
		/// <summary>
		/// returns an Vector::ConstIterator pointing to the head of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to the first element of the container (front)</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// returns an Vector::Iterator pointing past the end of the list.
		/// </summary>
		/// <returns>the iterator pointing to past the last element of the container (back)</returns>
		Iterator end();
		/// <summary>
		/// (const) returns an Vector::ConstIterator pointing past the end of the list.
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the constant container (back)</returns>
		ConstIterator end() const;
		/// <summary>
		/// returns an Vector::ConstIterator pointint past the end of the list (const only)
		/// Used primarily to get constant iterators on a non-constant container
		/// </summary>
		/// <returns>the constant iterator pointing to past the last element of the container (back)</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Reserves capacity amount of memory to later initialize when needed.
		/// </summary>
		/// <param name="capacity">how much capacity to reserve for Vector</param>
		void Reserve(size_t capacity);

		/// <summary>
		/// Resizes the vector to initialize data of vector up to size with default constructor.
		/// Will destuct all data after size if passed size is less than current container size.
		/// </summary>
		/// <param name="size">size to resize to</param>
		void Resize(size_t size);

		/// <summary>
		/// Completely clears the vector of its data leaving it empty, does not affect its capacity.
		/// </summary>
		void Clear();
		/// <summary>
		/// Shrinks capacity of the vector down to size.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Finds and returns first iterator pointing to item passed in, if not found it returns end.
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>Iterator pointing to index containing value or end if not found</returns>
		template<typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns first const iterator pointing to value item passed in, if not found it returns end.
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>Iterator pointing to index containing value or end if not found</returns>
		template<typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;

		/// <summary>
		/// Finds and Removes element of the vector containing the value. 
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
		/// <exception cref="runtime_error">throws exception if given iterator is not owned by container to insert rvalue to</exception>
		bool Remove(const Iterator& it);
		/// <summary>
		/// Removes the elements pointed within the range of iterators passed in.
		/// First inclusive and last exclusive.
		/// </summary>
		/// <param name="first">inclusive iterator pointing to begining of range to remove</param>
		/// <param name="last">exclusive iterator pointing to end of range to remove</param>
		/// <returns>true if remove of range was successful false otherwise</returns>
		bool Remove(const Iterator& first, const Iterator& last);


	private:
		T* _data{ nullptr };
		size_t _size{ 0_z };
		size_t _capacity{ 0_z };
	};
}

#include "Vector.inl"