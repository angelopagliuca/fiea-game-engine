#include "pch.h"

#include "ActionList.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() :
		IAction(ActionList::TypeIdClass())
	{
	}

	ActionList::ActionList(const std::string& name) :
		IAction{ ActionList::TypeIdClass() }
	{
		SetName(name);
	}

	ActionList::ActionList(RTTI::IdType type) :
		IAction{ type }
	{
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionList, _name) },
			{ "Actions"s, Datum::DatumType::Table, 0, 0 },
		};
	}

	IAction* ActionList::CreateAction(const std::string& factory, const std::string& name)
	{
		Scope* scope = Factory<Scope>::Create(factory);
		assert(scope != nullptr);
		IAction* action = scope->As<IAction>();
		assert(action != nullptr);
		action->SetName(name);
		Adopt(*action, "Actions");
		return action;
	}

	void ActionList::Update(WorldState& worldState)
	{
		worldState.Action = this;
		Datum& actions = Actions();

		for (size_t i = 0_z; i < actions.Size(); ++i)
		{
			Scope& scope = actions[i];
			assert(scope.Is(IAction::TypeIdClass()));
			IAction* action = static_cast<IAction*>(&scope);
			action->Update(worldState);
		}
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		const ActionList* other = rhs->As<ActionList>();
		return other != nullptr ? *this == *other : false;
	}

	std::string ActionList::ToString() const
	{
		return "ActionList"s;
	}

	Datum& ActionList::Actions() const
	{
		return _orderVector[actionsIndex]->second;
	}
}
