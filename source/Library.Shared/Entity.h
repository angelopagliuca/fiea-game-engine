#pragma once

#include "Attributed.h"
#include "IAction.h"
#include "WorldState.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Another wrapper for a table (i.e. an Attributed Scope) – with the difference that Entity objects
	/// automatically populate their tables based on information known at compile time.
	/// </summary>
	class Entity : public Attributed 
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		Entity();
		/// <summary>
		/// Constructs an Entity with this name
		/// </summary>
		/// <param name="name">name of entity to set to</param>
		Entity(const std::string& name);
		/// <summary>
		/// Root Event Constructor
		/// </summary>
		/// <param name="eQueue">eventQueue</param>
		/// <param name="gameTime">gameTime</param>
		Entity(EventQueue& eQueue, GameTime gameTime);
		Entity(const Entity& other) = default;
		Entity(Entity&& other) noexcept = default;
		Entity& operator=(const Entity& other) = default;
		Entity& operator=(Entity&& other) noexcept = default;
		~Entity() = default;

		/// <summary>
		/// Name of the Constructor
		/// </summary>
		/// <returns></returns>
		const std::string& Name() const;
		/// <summary>
		/// Children of the entity
		/// </summary>
		/// <returns>all the children of the entity</returns>
		Datum& Children() const;
		/// <summary>
		/// Actions of the entity
		/// </summary>
		/// <returns>all the actions of the entity</returns>
		Datum& Actions() const;

		/// <summary>
		/// Sets the name of the constructor
		/// </summary>
		/// <param name="name">name to set</param>
		void SetName(const std::string& name);
		/// <summary>
		/// Takes an entity class name and its instance name, and uses the entity factory to make a new
		/// object of the given type, adopts the entity into the sector, and returns the address of the new entity.
		/// </summary>
		/// <param name="factory">factory to use Create with</param>
		/// <param name="name">name of the entity</param>
		/// <returns>address of the new entity</returns>
		Entity* CreateChild(const std::string& factory, const std::string& name);
		/// <summary>
		/// Takes an action class name and its instance name, and uses the action factory to make a new
		/// object of the given type, adopts the action into the sector, and returns the address of the new action.
		/// </summary>
		/// <param name="factory">factory to use Create with</param>
		/// <param name="name">name of the action</param>
		/// <returns>address of the new action</returns>
		IAction* CreateAction(const std::string& factory, const std::string& name);

		/// <summary>
		/// Updates all the entities and its children
		/// </summary>
		/// <param name="worldState"></param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Clones the Entity (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<Entity*> Clone() const override;
		/// <summary>
		/// Equals function of the entity (virtual)
		/// </summary>
		/// <param name="rhs">other entity to compare</param>
		/// <returns>is other equal</returns>
		bool Equals(const RTTI * rhs) const override;
		/// <summary>
		/// ToString of the entity
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override;

		/// <summary>
		/// Signatures of the Entity Type
		/// </summary>
		/// <returns>vector of all signatures of the entity</returns>
		static const Vector<Signature> Signatures();

	protected:
		Entity(RTTI::IdType type);

		std::string _name;
		const static int childrenIndex{ 2 };
		const static int actionsIndex{ 3 };

		// For Root
		EventQueue* _eventQueue{ nullptr };
		GameTime _gameTime{};
	};

	ConcreteFactory(Entity, Scope);
}

