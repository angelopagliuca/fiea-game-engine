#pragma once

#include <gsl/gsl>

#include "Scope.h"
#include "TypeManager.h"
#include "SizeLiteral.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Attributed Class is the class we use in the game engine to express the “schema” at compile-time,
	/// to mirror native classes, since our Datum & Scope system allows us to create dynamic data structures at run-time.
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		/// <summary>
		/// Deleted since we don't want anyone instantiating
		/// this explicitly without an inherited class
		/// </summary>
		Attributed() = delete;
		/// <summary>
		/// Copy Constructor deep copies the other Attributed object.
		/// </summary>
		/// <param name="other">other Attributed object to copy</param>
		Attributed(const Attributed& other);
		/// <summary>
		/// Move Constructor moves the other Attributed object.
		/// </summary>
		/// <param name="other">other Attributed object to move</param>
		Attributed(Attributed&& other) noexcept;
		/// <summary>
		/// Copy Assignment Operator deep copies the other Attributed object.
		/// </summary>
		/// <param name="other">other Attributed object to copy</param>
		Attributed& operator=(const Attributed& other);
		/// <summary>
		/// Move Assignment Operator moves the other Attributed object.
		/// </summary>
		/// <param name="other">other Attributed object to move</param>
		/// <returns>moved constructor</returns>
		Attributed& operator=(Attributed&& other) noexcept;
		/// <summary>
		/// Destructor clears the attributed object and its underlying data.
		/// </summary>
		virtual ~Attributed() = default;

		/// <summary>
		/// Is the given string name an Attribute of the object?
		/// </summary>
		/// <param name="name">name to see if is an attribute</param>
		/// <returns>true if is an attribute false otherwise</returns>
		bool IsAttribute(const std::string& name) const;
		/// <summary>
		/// Is the given string name a Prescribed Attribute (part of the signature)
		/// of the object?
		/// </summary>
		/// <param name="name">name to see if is a prescribed attribute</param>
		/// <returns>true if is a prescribed attribute false otherwise</returns>
		bool IsPrescribedAttribute(const std::string& name) const;
		/// <summary>
		/// Is the given string name an Auxiliary Attribute
		/// (appended after instantiation) of the object?
		/// </summary>
		/// <param name="name">name to see if is an auxiliary attribute</param>
		/// <returns>true if is an auxiliary attribute false otherwise</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Given a string (which must not be the name of a prescribed attribute), 
		/// appends a Datum to this 'Scope' with the given name, and returns a reference to that Datum. 
		/// </summary>
		/// <param name="name">given name to append</param>
		/// <returns>reference to Datum appended</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Accessor method of all attributes.
		/// </summary>
		/// <returns>All attributes of this attributed object</returns>
		const Vector<Scope::PairType*>& Attributes() const;
		/// <summary>
		/// Accessor method of prescribed attributes.
		/// </summary>
		/// <returns>Only prescribe attributes of this attributed object</returns>
		Vector<Scope::PairType*> PrescribedAttributes() const;
		/// <summary>
		/// Accessor method of auxiliary attributes.
		/// </summary>
		/// <returns>Only auxiliary attributes of this attributed object</returns>
		Vector<Scope::PairType*> AuxiliaryAttributes() const;

	protected:
		explicit Attributed(RTTI::IdType typeId);

	private:
		void Populate(RTTI::IdType typeId);
		void UpdateExternalStorage(RTTI::IdType typeId);
	};
}

