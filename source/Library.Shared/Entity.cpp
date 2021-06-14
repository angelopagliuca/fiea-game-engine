#include "pch.h"

#include "Entity.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity);

	Entity::Entity() :
		Attributed(Entity::TypeIdClass())
	{
	}

	Entity::Entity(const std::string& name) :
		Attributed{ Entity::TypeIdClass() }, _name{ name }
	{
	}

	Entity::Entity(EventQueue& eQueue, GameTime gameTime) :
		Attributed{ Entity::TypeIdClass() }, _eventQueue{ &eQueue }, _gameTime{ gameTime }
	{
	}


	Entity::Entity(RTTI::IdType type) :
		Attributed{ type }
	{
	}

	const Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(Entity, _name) },
			{ "Children"s, Datum::DatumType::Table, 0, 0},
			{ "Actions"s, Datum::DatumType::Table, 0, 0 },
		};
	}


	const std::string& Entity::Name() const
	{
		return _name;
	}

	Datum& Entity::Children() const
	{
		return _orderVector[childrenIndex]->second;
	}

	Datum& Entity::Actions() const
	{
		return _orderVector[actionsIndex]->second;
	}

	void Entity::SetName(const std::string& name)
	{
		_name = name;
	}

	Entity* Entity::CreateChild(const std::string& factory, const std::string& name)
	{
		Scope* scope = Factory<Scope>::Create(factory);
		assert(scope != nullptr);
		Entity* child = scope->As<Entity>();
		child->SetName(name);
		Adopt(*child, "Children");
		return child;
	}

	IAction* Entity::CreateAction(const std::string& factory, const std::string& name)
	{
		Scope* scope = Factory<Scope>::Create(factory);
		assert(scope != nullptr);
		IAction* action = scope->As<IAction>();
		action->SetName(name);
		Adopt(*action, "Actions");
		return action;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.Entity = this;

		for (size_t i = 0_z; i < Children().Size(); ++i)
		{
			Scope& scope = Children()[i];
			assert(scope.Is("Entity"));
			Entity* child = static_cast<Entity*>(&scope);
			child->Update(worldState);
		}
		for (size_t i = 0_z; i < Actions().Size(); ++i)
		{
			Scope& scope = Actions()[i];
			assert(scope.Is("IAction"));
			IAction* action = static_cast<IAction*>(&scope);
			action->Update(worldState);
		}
	}

	gsl::owner<Entity*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	bool Entity::Equals(const RTTI* rhs) const
	{
		const Entity* other = rhs->As<Entity>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Entity::ToString() const
	{
		return "Entity"s;
	}
}
