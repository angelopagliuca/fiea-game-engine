#pragma once

#include <gsl/gsl>
#include <limits>

#include "HashMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Our configuration language allows us to express the intention of creating objects, based on the name
	/// of their type. Factory, is suitable for use with any interface class that has a default constructor.
	/// The game engine will use this Factory to create objects by their type name.
	/// </summary>
	template <typename T>
	class Factory
	{
	public:
		/// <summary>
		/// Returns a string representing the name of the class the factory instantiates.
		/// </summary>
		/// <returns>string representing the name of the class</returns>
		virtual const std::string& ClassName() const = 0;
		/// <summary>
		/// Pure virtual create function
		/// </summary>
		/// <returns>new object of that type given</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Given a class name (string), return the associated concrete factory.
		/// </summary>
		/// <param name="className">given class name</param>
		/// <returns>the associated concrete factory</returns>
		static const Factory* const Find(const std::string& className);

		/// <summary>
		/// Given a class name (string), returns a new object of that type.
		/// </summary>
		/// <param name="className">given class name</param>
		/// <returns>new object of that type given</returns>
		static gsl::owner<T*> Create(const std::string& className);
		/// <summary>
		/// resizes the factory map of all factories
		/// </summary>
		/// <param name="bucketSize">new size</param>
		static void Resize(size_t bucketSize);

		/// <summary>
		/// returns number of factories
		/// </summary>
		/// <returns>number of factories</returns>
		static size_t Size();
		/// <summary>
		/// is factories empty?
		/// </summary>
		/// <returns>true if no factories false otherwise</returns>
		static bool IsEmpty();

	protected:
		static void Add(const Factory& factory);
		static void Remove(const Factory& factory);

	private:
		inline static HashMap<std::string, const Factory* const> _factories;
	};
}

/// <summary>
/// ConcreteFactory is the macro used to create derived factories
/// </summary>
#define ConcreteFactory(ConcreteProductT, AbstractProductT)												\
	class ConcreteProductT##Factory final : public FieaGameEngine::Factory<AbstractProductT>			\
	{																									\
	public:																								\
		ConcreteProductT##Factory() { FieaGameEngine::Factory<AbstractProductT>::Add(*this); }			\
		~ConcreteProductT##Factory() { FieaGameEngine::Factory<AbstractProductT>::Remove(*this); }		\
		const std::string& ClassName() const override { return _className; }							\
		gsl::owner<ConcreteProductT*> Create() const override { return new ConcreteProductT(); }		\
	private:																							\
		inline static const std::string _className{ #ConcreteProductT };								\
	};																									

#include "Factory.inl"