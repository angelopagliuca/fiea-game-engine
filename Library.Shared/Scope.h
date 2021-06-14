#pragma once

#include <string>
#include <gsl/gsl>

#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"
#include "Factory.h"
#include "RTTI.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Scope objects are tables that create dictionary of name-value
	/// pairs where Datum objects are the values. Each entry in a Scope
	/// table has a name and a Datum, where the Datum represents an array
	/// of values of a single type. Furthermore, an entry in a Scope table
	/// can refer to another Scope table and thereby provides the means to
	/// create user-defined types which are a Datum type.  So the Datum & Scope
	/// classes form a recursive pair: Scopes are tables of Datum, some of
	/// which can be other tables (i.e. Scopes). Also, since each Scope has
	/// a pointer to its parent, this forms a tree of Scopes. 
	/// </summary>
	class Scope : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		/// <summary>
		/// Constructor for Scope. Initializes underlying map and vector.
		/// If optional capacity passed in it pre allocates the memory for the
		/// map and vector to that capacity.
		/// </summary>
		/// <param name="capacity">pre allocated size of vector</param>
		explicit Scope(size_t size = 0);
		/// <summary>
		/// Copy Constructor deep copies the other Scope. It does not
		/// link to the parent though.
		/// </summary>
		/// <param name="other">other Scope to copy</param>
		Scope(const Scope& other);
		/// <summary>
		/// Move Constructor moves the other Scope. If the other scope has a parent
		/// then it lets the parent know that it has been moved to this one. If it has
		/// scope children then it also lets them know that they have a new parent.
		/// </summary>
		/// <param name="other">other scope to move</param>
		Scope(Scope&& other) noexcept;
		/// <summary>
		/// Copy Assignment Operator deep copies the other Scope. It does not
		/// link to the parent though.
		/// </summary>
		/// <param name="other">other Scope to copy</param>
		Scope& operator=(const Scope& other);
		/// <summary>
		/// Move Assignment Operator moves the other Scope. If the other scope has a parent
		/// then it lets the parent know that it has been moved to this one. If it has
		/// scope children then it also lets them know that they have a new parent.
		/// </summary>
		/// <param name="other">other scope to move</param>
		/// <returns>moved constructor</returns>
		Scope& operator=(Scope&& other) noexcept;
		/// <summary>
		/// Destructor clears scope and its underlying map and vector.
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Takes a const reference to a Scope and returns a bool indicating that the two
		/// Scope objects have matching contents.
		/// </summary>
		/// <param name="other">other Scope to compare to</param>
		/// <returns>bool indicating that the two Scope objects have matching contents</returns>
		bool operator==(const Scope& other) const;
		/// <summary>
		/// Takes a const reference to a Scope and returns a bool indicating that the two
		/// Scope objects do not have matching contents.
		/// </summary>
		/// <param name="other">other Scope to compare to</param>
		/// <returns>bool indicating that the two Scope objects do not have matching contents</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Takes an unsigned integer and which returns a reference to a Datum at the given index.
		/// The index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">given index to get Datum from</param>
		/// <returns>Reference of Datum at given input</returns>
		Datum& operator[](size_t index);
		/// <summary>
		/// Takes an unsigned integer and which returns a constant reference to a Datum at the given index.
		/// The index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">given index to get Datum from</param>
		/// <returns>Constant reference of Datum at given input</returns>
		const Datum& operator[](size_t index) const;
		/// <summary>
		/// Takes a constant string and which wraps Append, for syntactic convenience.
		/// </summary>
		/// <param name="name">string to append to</param>
		/// <returns>datum reference that was appended or found</returns>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Size of the Scope.
		/// </summary>
		/// <returns>Size of the Scope</returns>
		size_t Size() const;
		/// <summary>
		/// Is Scope Empty?
		/// </summary>
		/// <returns>Size == 0</returns>
		bool IsEmpty() const;
		
		/// <summary>
		/// Deletes all memory allocated by this object
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns the address of the Scope which contains this scope.
		/// </summary>
		/// <returns>address of the Scope which contains this scope</returns>
		Scope* GetParent();
		/// <summary>
		/// Returns the constant address of the Scope which contains this scope.
		/// </summary>
		/// <returns>constant address of the Scope which contains this scope</returns>
		const Scope* GetParent() const;

		/// <summary>
		/// Takes a constant string and returns a reference to a Datum with the associated name.
		/// If it already exists, return that one, otherwise create one. 
		/// Note that the order in which values are appended must be preserved. 
		/// </summary>
		/// <param name="name">Given key to append</param>
		/// <returns>Reference to a Datum with the associated name</returns>
		/// <exception cref="invalid_argument">Cannot Append an empty string name into Scope</exception>
		Datum& Append(const std::string& name);
		/// <summary>
		/// Takes a constant string and an address to a bool. If entry created bool sets to true.
		/// Returns a reference to a Datum with the associated name.
		/// If it already exists, return that one, otherwise create one. 
		/// Note that the order in which values are appended must be preserved. 
		/// </summary>
		/// <param name="name">Given key to append</param>
		/// <param name="entryCreated">sets to true if entry was created false otherwise</param>
		/// <returns>Reference to a Datum with the associated name</returns>
		/// <exception cref="invalid_argument">Cannot Append an empty string name into Scope</exception>
		Datum& Append(const std::string& name, bool& entryCreated);
		/// <summary>
		/// Takes a constant string and returns a reference to Scope. Returns a reference to a Scope
		/// with the associated name. If a Datum already exists at that key it reuses it, otherwise it
		/// creates a new Datum. Note that AppendScope is a special case of Append and therefore, as with
		/// Append, the order of values added will be preserved.
		/// </summary>
		/// <param name="name">Given key to append scope to</param>
		/// <returns>Reference to a Scope with the associated name</returns>
		/// <exception cref="invalid_argument">Cannot Append an empty string name into Scope</exception>
		/// <exception cref="runtime_error">Cannot Append a Scope to a Datum with invalid type</exception>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Takes the constant address of a Scope and returns the Datum pointer and index at which the Scope was found.
		/// This function looks at all children contained within this scope to find given scope.
		/// </summary>
		/// <param name="scope">Scope to be found</param>
		/// <returns>Datum pointer and index at which the Scope was found or datum as a nullptr if not found</returns>
		const std::pair<Datum*, size_t> FindContainedScope(const Scope& scope) const;

		/// <summary>
		/// Takes a constant string and returns the address of a Datum. This should return the address of
		/// the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to find in Scope</param>
		/// <returns>Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.</returns>
		Datum* Find(const std::string& name);
		/// <summary>
		/// Takes a constant string and returns the address of a Datum. This should return the address of
		/// the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to find in Scope</param>
		/// <returns>Constant datum associated with the given name in this Scope, if it exists, and nullptr otherwise.</returns>
		const Datum* Find(const std::string& name) const;
		/// <summary>
		/// Takes a constant string to search in scope.
		/// Return the address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to search</param>
		/// <returns>Address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.</returns>
		Datum* Search(const std::string& name);
		/// <summary>
		/// Takes a constant string to search in scope.
		/// Return the const address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to search</param>
		/// <returns>Constant address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.</returns>
		const Datum* Search(const std::string& name) const;
		/// <summary>
		/// Takes a constant string and the address of a Scope double pointer variable.
		/// Return the address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to search</param>
		/// <param name="foundScope">If provided, shall contain the address of the Scope object which contains the match.</param>
		/// <returns>Address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.</returns>
		Datum* Search(const std::string& name, Scope*& foundScope);
		/// <summary>
		/// Takes a constant string and the address of a Scope double pointer variable.
		/// Return the constant address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="name">Given key to search</param>
		/// <param name="foundScope">If provided, shall contain the address of the Scope object which contains the match.</param>
		/// <returns>Constant address of the most-closely nested Datum associated with the given
		/// name in this Scope or its ancestors, if it exists, and nullptr otherwise.</returns>
		const Datum* Search(const std::string& name, const Scope*& foundScope) const;


		Datum& At(const std::string& name);

		const Datum& At(const std::string& name) const;

		/// <summary>
		/// Adopts scope given by parenting it at the given key as a name.
		/// </summary>
		/// <param name="scope">Child to adopt</param>
		/// <param name="name">name of key for the Datum to use for storing the child</param>
		/// <exception cref="runtime_error">Cannot self-Adopt</exception>
		/// <exception cref="runtime_error">Cannot Adopt ancestor Scope</exception>
		void Adopt(Scope& scope, const std::string& name);
		/// <summary>
		/// Unparents the scope. Orphaned scope now has no owner so whoever orphaned it must delete it.
		/// </summary>
		void Orphan();

		/// <summary>
		/// RTTIs overriden Equals function. This function works the same as operator ==.
		/// </summary>
		/// <param name="rhs">other scope to compare</param>
		/// <returns>true if rhs is equivalent to this scope</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// RTTIs overriden ToString that converts this scope into a string format.
		/// </summary>
		/// <returns>string representing this scope</returns>
		std::string ToString() const override;

		/// <summary>
		/// Clone creates new copy of Scope, we need it virtual since Scope
		/// is not final and every inherited class must have a Clone that
		/// will call its Clone.
		/// </summary>
		/// <returns>new Scope</returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Is this scope an ancestor of the given scope?
		/// </summary>
		/// <param name="scope">scope to check if this is an ancestor of</param>
		/// <returns>if this scope is an ancestor</returns>
		bool IsAncestorOf(const Scope& scope) const;
		/// <summary>
		/// Is this scope a descendant of the given scope?
		/// </summary>
		/// <param name="scope">scope to check if this is an descendant of</param>
		/// <returns>if this scope is an descendant</returns>
		bool IsDescendantOf(const Scope& scope) const;

	protected:
		using MapType = HashMap<std::string, Datum>;
		using PairType = MapType::PairType;

		void Reparent(Scope&& rhs);
		void DeepCopy(const Scope& other);

		using NestedScopeFunction = std::function<bool(const Scope&, Datum&, size_t)>;
		void ForEachNestedScopeIn(NestedScopeFunction func) const;

		Scope* _parent{ nullptr };
		MapType _map;
		Vector<PairType*> _orderVector;
	};

	ConcreteFactory(Scope, Scope);
}
