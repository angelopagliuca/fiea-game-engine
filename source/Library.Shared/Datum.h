#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <initializer_list>

#include "DefaultIncrement.h"
#include "DefaultEquality.h"
#include "HashMap.h"
#include "RTTI.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
	class Scope;
	class Attributed;

	/// <summary>
	/// Datum is a vector of values. The values given in Datum have a single type - either
	/// a primitive type or a user-defined type. It is able to represent a reference to
	/// a Scope, and so the pair forms a recursive data structure, able to represent arbitrary
	/// data structure topologies akin to classes in an object-oriented programming language. 
	/// </summary>
	class Datum final
	{
		friend Scope;
		friend Attributed;

	public:
		/// <summary>
		/// Types supported by Datum class.
		/// </summary>
		enum class DatumType {
			Integer,
			Float,
			Vector,
			Matrix,
			Table,
			String,
			Pointer,
			Unknown,
		};

		/// <summary>
		/// Initializes Datum to be empty. Sets Datum to specific type if a DatumType is
		/// passed in as a parameter. Can't change later if not Unknown.
		/// </summary>
		/// <param name="type">Type to set Datum to</param>
		explicit Datum(DatumType type = DatumType::Unknown);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of ints.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<int> list);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of floats.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<float> list);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of glm vectors.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<glm::vec4> list);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of glm matrices.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<glm::mat4> list);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of STL strings.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<std::string> list);
		/// <summary>
		/// Creates new instance of datum vector container using an initializer list of RTTI pointers.
		/// Size and capacity will be size of list.
		/// </summary>
		/// <param name="list">initializer list</param>
		Datum(std::initializer_list<RTTI*> list);
		/// <summary>
		/// Creates new instance of datum vector container using an integer scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(const int& value);
		/// <summary>
		/// Creates new instance of datum vector container using a float scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(const float& value);
		/// <summary>
		/// Creates new instance of datum vector container using a vector scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(const glm::vec4& value);
		/// <summary>
		/// Creates new instance of datum vector container using a matrix scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(const glm::mat4& value);
		/// <summary>
		/// Creates new instance of datum vector container using a string scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(const std::string& value);
		/// <summary>
		/// Creates new instance of datum vector container using a pointer scalar value.
		/// Size and capacity will be one.
		/// </summary>
		/// <param name="list">scalar value</param>
		Datum(RTTI*& value);

		/// <summary>
		/// Copy Constructor for new instance of datum vector container.
		/// Deep copies other datum vector passed in.
		/// Capacity will be the same capacity as other datum.
		/// </summary>
		/// <param name="other">other datum vector to copy from</param>
		Datum(const Datum& other);
		/// <summary>
		/// Move Constructor for new instance of datum vector container.
		/// Moves other datum vector passed in, making other datum vector invalid.
		/// Capacity will be the same capacity as other datum.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		Datum(Datum&& other) noexcept;
		/// <summary>
		/// Copy assigment operator for datum vector container.
		/// Clears current datum vector and copies over the other datum vector.
		/// </summary>
		/// <param name="other">other datum to copy</param>
		/// <returns>copy of the other datum</returns>
		Datum& operator=(const Datum& other);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of ints.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<int> list);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of floats.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<float> list);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of vectors.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<glm::vec4> list);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of matrices.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<glm::mat4> list);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of strings.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<std::string> list);
		/// <summary>
		/// Copy operator for datum vector container using initializer list of pointers.
		/// Clears current datum vector and copies over the initializer list.
		/// </summary>
		/// <param name="list">initializer list to copy</param>
		/// <returns>copy of the initializer list as datum</returns>
		Datum& operator=(std::initializer_list<RTTI*> list);
		/// <summary>
		/// Copy operator for datum vector container using a scalar int.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(const int& value);
		/// <summary>
		/// Copy operator for datum vector container using a scalar float.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(const float& value);
		/// <summary>
		/// Copy operator for datum vector container using a scalar glm vector.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(const glm::vec4& value);
		/// <summary>
		/// Copy operator for datum vector container using a scalar glm matrix.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(const glm::mat4& value);
		/// <summary>
		/// Copy operator for datum vector container using a scalar scope.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(Scope& value);
		/// <summary>
		/// Copy operator for datum vector container using a single string.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(const std::string& value);
		/// <summary>
		/// Copy operator for datum vector container using a single pointer.
		/// Clears current datum vector and makes size and capacity 1,
		/// with value passed in.
		/// </summary>
		/// <param name="value">scalar value to copy</param>
		/// <returns>copy of scalar value as datum</returns>
		Datum& operator=(RTTI* const& value);
		/// <summary>
		/// Move operator for datum vector container.
		/// Clears current datum vector and moves over the other datum vector.
		/// </summary>
		/// <param name="other">other datumvector to copy</param>
		/// <returns>moved datum vector</returns>
		Datum& operator=(Datum&& other) noexcept;
		/// <summary>
		/// Destructs Datum container destructing elements and freeing data.
		/// </summary>
		~Datum();

		/// <summary>
		/// Returns Datum type associated with this datum.
		/// </summary>
		/// <returns>type enum associated with this datum</returns>
		DatumType Type() const;
		/// <summary>
		/// Assigns the type of data this object stores with the given type Enum.
		/// </summary>
		/// <param name="type">Type to assign data to.</param>
		/// <exception cref="runtime_error">Exception thrown if invalid type or changed type.</exception>
		void SetType(DatumType type);

		/// <summary>
		/// Size of the Datum Vector
		/// </summary>
		/// <returns>the size of the datum container</returns>
		size_t Size() const;
		/// <summary>
		/// capicity of the datum vector
		/// </summary>
		/// <returns>the capacity of the container</returns>
		size_t Capacity() const;
		/// <summary>
		/// is datum external?
		/// </summary>
		/// <returns>isExternal</returns>
		bool IsExternal() const;

		/// <summary>
		/// is datum empty?
		/// </summary>
		/// <returns>isEmpty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Reserves capacity amount of memory to later initialize when needed.
		/// </summary>
		/// <param name="capacity">how much capacity to reserve for Datum</param>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		void Reserve(size_t capacity);
		/// <summary>
		/// Resizes the Datum Vector to initialize data of datum vector up to size with default constructor.
		/// Will destuct all data after size if passed size is less than current container size.
		/// </summary>
		/// <param name="size">size to resize to</param>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		void Resize(size_t size);

		/// <summary>
		/// Completely clears the datum vector of its data leaving it empty, does not affect its capacity.
		/// </summary>
		void Clear();

		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(int* data, size_t size);
		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(float* data, size_t size);
		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(glm::vec4* data, size_t size);
		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(glm::mat4* data, size_t size);
		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(std::string* data, size_t size);
		/// <summary>
		/// Given an array and the number of elements in it, it assigna the internal
		/// values array to the given array, and the size and capacity to the given
		/// number of elements.
		/// </summary>
		void SetStorage(RTTI** data, size_t size);

		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const Datum& other) const;
		/// <summary>
		/// The datum containers are not equal if the sizes are not the same and
		/// if the internal elements are not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const Datum& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const int& other) const;
		/// <summary>
		/// The datum is not equal to the int if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const int& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const float& other) const;
		/// <summary>
		/// The datum is not equal to the float if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const float& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const glm::vec4& other) const;
		/// <summary>
		/// The datum is not equal to the vec4 if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const glm::vec4& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const glm::mat4& other) const;
		/// <summary>
		/// The datum is not equal to the mat4 if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const glm::mat4& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(const std::string& other) const;
		/// <summary>
		/// The datum is not equal to the string if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(const std::string& other) const;
		/// <summary>
		/// !(operator!=(other))
		/// </summary>
		/// <param name="other"></param>
		/// <returns>!(operator!=(other))</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator==(RTTI* const& other) const;
		/// <summary>
		/// The datum is not equal to the RTTI* if the size is not 1 and
		/// if the internal element is not the same either.
		/// </summary>
		/// <param name="other">other datum container to compare to</param>
		/// <returns>true if not equal and false otherwise</returns>
		/// <exception cref="runtime_error">Exception thrown if datum has unknown type.</exception>
		bool operator!=(RTTI* const& other) const;

		/// <summary>
		/// Syntactic sugar for dereferencing nested Scopes
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>scope reference</returns>
		Scope& operator[](size_t index);
		/// <summary>
		/// Syntactic sugar for dereferencing nested Scopes
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>scope reference</returns>
		const Scope& operator[](size_t index) const;

		/// <summary>
		/// Given an int value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not Integer</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(const int& value, size_t index = 0);
		/// <summary>
		/// Given a float value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not Float</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(const float& value, size_t index = 0);
		/// <summary>
		/// Given a vec4 value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not Vector</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(const glm::vec4& value, size_t index = 0);
		/// <summary>
		/// Given a mat4 value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not Matrix</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(const glm::mat4& value, size_t index = 0);
		/// <summary>
		/// Given a string value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not String</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(const std::string& value, size_t index = 0);
		/// <summary>
		/// Given a RTTI* value and an optional index (default is 0), it assigns
		/// the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">value to set</param>
		/// <param name="index">index in array to set</param>
		/// <exception cref="runtime_error">exception thrown if datum type not Pointer</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		void Set(RTTI* const& value, size_t index = 0);

		/// <summary>
		/// Given an optional index (default is 0), it returns the int value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		int& GetInteger(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the float value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		float& GetFloat(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the vector value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		glm::vec4& GetVector(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the matrix value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		glm::mat4& GetMatrix(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the scope value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		Scope& GetScope(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the string value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		std::string& GetString(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the pointer value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		RTTI*& GetPointer(size_t index = 0);
		/// <summary>
		/// Given an optional index (default is 0), it returns the int value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const int& GetInteger(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the float value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const float& GetFloat(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the vector value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const glm::vec4& GetVector(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the matrix value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const glm::mat4& GetMatrix(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the const scope value from the values array.
		/// </summary>
		/// <returns>const reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const Scope& GetScope(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the string value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		const std::string& GetString(size_t index = 0) const;
		/// <summary>
		/// Given an optional index (default is 0), it returns the pointer value from the values array.
		/// </summary>
		/// <returns>reference of value at index</returns>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		/// <exception cref="out_of_range">exception thrown if index out of bounds</exception>
		RTTI* const& GetPointer(size_t index = 0) const;


		const int FromStringToInt(const std::string& str) const;
		const float FromStringToFloat(const std::string& str) const;
		const glm::vec4 FromStringToVector(const std::string& str) const;
		const glm::mat4 FromStringToMatrix(const std::string& str) const;

		/// <summary>
		/// Given an STL string and an optional index (default 0) it assigns
		/// the appropriate element in the values array to the given value.
		/// (enforce that the datum’s type has already been specified).
		/// </summary>
		/// <param name="string">string to parse</param>
		/// <param name="index">index to set</param>
		/// <exception cref="runtime_error">cannot set from string with an unknown or pointer type</exception>
		void SetFromString(const std::string& str, size_t index = 0);

		/// <summary>
		/// Given an optional index (default is 0) returns an STL string
		/// representing the appropriate element in the values array.
		/// </summary>
		/// <param name="index">index of value to return as string</param>
		/// <returns>string of value at index</returns>
		/// <exception cref="runtime_error">cannot convert to string with an unknown type</exception>
		std::string ToString(size_t index = 0) const;


		template<typename IncrementFunctor = FieaGameEngine::DefaultIncrement>
		void PushBackFromString(const std::string& str);

		/// <summary>
		/// Pushes the int data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = FieaGameEngine::DefaultIncrement>
		void PushBack(const int& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the float data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(const float& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the vec4 data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(const glm::vec4& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the mat4 data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(const glm::mat4& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the string data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(const std::string& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the pointer data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">lvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(RTTI* const& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the int data (rvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(int&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the float data (rvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(float&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the vec4 data (rvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(glm::vec4&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the mat4 data (rvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(glm::mat4&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the string data (lvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(std::string&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// Pushes the pointer data (rvalue) into the back of the datum vector.
		/// You can either use the default increment provided or pass
		/// in your own functor. Will increase capacity by at least 1
		/// if not enough capacity to guarantee push.
		/// </summary>
		/// <param name="value">rvalue data</param>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown or different</exception>
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(RTTI*&& value, IncrementFunctor incrementFunctor = IncrementFunctor{});
		/// <summary>
		/// removes the last item in the container [datum], but does not reduce the capacity of the container.
		/// </summary>
		/// <exception cref="runtime_error">exception thrown if datum type is unknown</exception>
		void PopBack();

		/// <summary>
		/// returns the int data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		int& FrontInteger();
		/// <summary>
		/// returns the int data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		float& FrontFloat();
		/// <summary>
		/// returns the int data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		glm::vec4& FrontVector();
		/// <summary>
		/// returns the int data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		glm::mat4& FrontMatrix();
		/// <summary>
		/// returns the int data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		std::string& FrontString();
		/// <summary>
		/// returns the pointer data of the front element as a reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		RTTI*& FrontPointer();
		/// <summary>
		/// returns the int data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const int& FrontInteger() const;
		/// <summary>
		/// returns the int data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const float& FrontFloat() const;
		/// <summary>
		/// returns the int data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const glm::vec4& FrontVector() const;
		/// <summary>
		/// returns the int data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const glm::mat4& FrontMatrix() const;
		/// <summary>
		/// returns the int data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const std::string& FrontString() const;
		/// <summary>
		/// returns the pointer data of the front element as a const reference
		/// </summary>
		/// <returns>returns the element at the front of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		RTTI* const& FrontPointer() const;

		/// <summary>
		/// returns the int data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		int& BackInteger();
		/// <summary>
		/// returns the int data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		float& BackFloat();
		/// <summary>
		/// returns the int data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		glm::vec4& BackVector();
		/// <summary>
		/// returns the int data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		glm::mat4& BackMatrix();
		/// <summary>
		/// returns the int data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		std::string& BackString();
		/// <summary>
		/// returns the pointer data of the Back element as a reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		RTTI*& BackPointer();
		/// <summary>
		/// returns the int data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const int& BackInteger() const;
		/// <summary>
		/// returns the int data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const float& BackFloat() const;
		/// <summary>
		/// returns the int data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const glm::vec4& BackVector() const;
		/// <summary>
		/// returns the int data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const glm::mat4& BackMatrix() const;
		/// <summary>
		/// returns the int data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		const std::string& BackString() const;
		/// <summary>
		/// returns the pointer data of the Back element as a const reference
		/// </summary>
		/// <returns>returns the element at the Back of the container</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		/// <exception cref="runtime_error">throws exception if datum is empty</exception>
		RTTI* const& BackPointer() const;

		/// <summary>
		/// Finds and Removes element of the datum containing the int value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<int>>
		void Remove(const int& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and Removes element of the datum containing the float value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<float>>
		void Remove(const float& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and Removes element of the datum containing the vec4 value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<glm::vec4>>
		void Remove(const glm::vec4& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and Removes element of the datum containing the mat value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<glm::mat4>>
		void Remove(const glm::mat4& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and Removes element of the datum containing the string value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<std::string>>
		void Remove(const std::string& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and Removes element of the datum containing the pointer value. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		template<typename EqualityFunctor = DefaultEquality<RTTI *>>
		void Remove(RTTI* const& value, EqualityFunctor equalityFunctor = EqualityFunctor{});

		/// <summary>
		/// Finds and Removes element of the datum at index passed. 
		/// </summary>
		/// <param name="value">value to find and remove</param>
		/// <returns>true if remove was successful false otherwise</returns>
		void RemoveAt(size_t index);

		/// <summary>
		/// Finds and returns index of int passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<int>>
		size_t IndexOf(const int& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns index of float passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<float>>
		size_t IndexOf(const float& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns index of glm::vec4 passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<glm::vec4>>
		size_t IndexOf(const glm::vec4& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns index of glm::mat4 passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<glm::mat4>>
		size_t IndexOf(const glm::mat4& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns index of STL string passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<std::string>>
		size_t IndexOf(const std::string& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns index of pointer passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<RTTI*>>
		size_t IndexOf(RTTI* const& value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		/// <summary>
		/// Finds and returns const index of to value int passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<int>>
		const size_t IndexOf(const int& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		/// <summary>
		/// Finds and returns const index of to value float passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<float>>
		const size_t IndexOf(const float& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		/// <summary>
		/// Finds and returns const index of to value glm::vec4 passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<glm::vec4>>
		const size_t IndexOf(const glm::vec4& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		/// <summary>
		/// Finds and returns const index of to value glm::mat4 passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<glm::mat4>>
		const size_t IndexOf(const glm::mat4& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		/// <summary>
		/// Finds and returns const index of to value STL string passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<std::string>>
		const size_t IndexOf(const std::string& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		/// <summary>
		/// Finds and returns const index of to value pointer passed in, if not found it returns Size().
		/// Most use an equality functor in order to compare equality of element type of the container.
		/// </summary>
		/// <param name="value">value to find in vector</param>
		/// <returns>index containing value or size if not found</returns>
		/// <exception cref="runtime_error">throws exception if datum is unknown or not same type</exception>
		template<typename EqualityFunctor = DefaultEquality<RTTI*>>
		const size_t IndexOf(RTTI* const& value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;

		static const HashMap<std::string, DatumType> DatumTypeMap;


	private:
		union DatumValue {
			int* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			Scope** t;
			std::string* s;
			RTTI** p;
			void* vp;
		};

		DatumType _type{ DatumType::Unknown };
		DatumValue _data{ nullptr };
		size_t _size{ 0_z };
		size_t _capacity{ 0_z};
		bool _isExternal = false;

		void Set(Scope& value, size_t index = 0);
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBack(Scope& value, IncrementFunctor incrementFunctor = IncrementFunctor{});

		void SetStorage(DatumType type, void* data, size_t size);
		void SetHelper(DatumType type, size_t index);
		void GetHelper(DatumType type, size_t index) const;
		template<typename IncrementFunctor = DefaultIncrement>
		void PushBackHelper(DatumType type, IncrementFunctor incrementFunctor = IncrementFunctor{});
		void FrontHelper(DatumType type) const;
		void BackHelper(DatumType type) const;

		static const size_t _sizeMap[static_cast<int>(DatumType::Unknown)];

		void CreateIntegers(size_t start, size_t size) const;
		void CreateFloats(size_t start, size_t size) const;
		void CreateVectors(size_t start, size_t size) const;
		void CreateMatrixes(size_t start, size_t size) const;
		void CreateStrings(size_t start, size_t size) const;
		void CreatePointers(size_t start, size_t size) const;

		using CreateFunctor = void (Datum::*) (size_t start, size_t size) const;
		static const CreateFunctor _createFunctors[static_cast<int>(DatumType::Unknown) + 1];

		void CopyIntegers(DatumValue data, size_t size) const;
		void CopyFloats(DatumValue data, size_t size) const;
		void CopyVectors(DatumValue data, size_t size) const;
		void CopyMatrixes(DatumValue data, size_t size) const;
		void CopyStrings(DatumValue data, size_t size) const;
		void CopyPointers(DatumValue data, size_t size) const;

		using CopyFunctor = void (Datum::*) (DatumValue data, size_t size) const;
		static const CopyFunctor _copyFunctors[static_cast<int>(DatumType::Unknown) + 1];

		bool ComparePODs(const Datum& rhs) const;
		bool CompareStrings(const Datum& rhs) const;
		bool ComparePointers(const Datum& rhs) const;

		using CompareFunctor = bool (Datum::*) (const Datum& rhs) const;
		static const CompareFunctor _compareFunctors[static_cast<int>(DatumType::Unknown) + 1];

		void SetFromStringIntegers(const std::string& str, size_t index);
		void SetFromStringFloats(const std::string& str, size_t index);
		void SetFromStringVectors(const std::string& str, size_t index);
		void SetFromStringMatrixes(const std::string& str, size_t index);
		void SetFromStringStrings(const std::string& str, size_t index);

		using SetFromStringFunctor = void (Datum::*) (const std::string& str, size_t index);
		static const SetFromStringFunctor _setFromStringFunctors[static_cast<int>(DatumType::Unknown) + 1];

		std::string ToStringIntegers(size_t index) const;
		std::string ToStringFloats(size_t index) const;
		std::string ToStringVectors(size_t index) const;
		std::string ToStringMatrixes(size_t index) const;
		std::string ToStringStrings(size_t index) const;
		std::string ToStringPointers(size_t index) const;

		using ToStringFunctor = std::string (Datum::*) (size_t index) const;
		static const ToStringFunctor _toStringFunctors[static_cast<int>(DatumType::Unknown) + 1];

		void PushBackFromStringIntegers(const std::string& str);
		void PushBackFromStringFloats(const std::string& str);
		void PushBackFromStringVectors(const std::string& str);
		void PushBackFromStringMatrixes(const std::string& str);
		void PushBackFromStringStrings(const std::string& str);

		using PushBackFromStringFunctor = void (Datum::*) (const std::string& str);
		static const PushBackFromStringFunctor _pushBackFromStringFunctors[static_cast<int>(DatumType::Unknown) + 1];
	};
}

#include "Datum.inl"

