#include "pch.h"

#include "ActionDestroyAction.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	ActionDestroyAction::ActionDestroyAction() :
		IAction(ActionDestroyAction::TypeIdClass())
	{
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionDestroyAction, _name) },
		};
	}


	void ActionDestroyAction::Update(WorldState& worldState)
	{
		worldState.Action = this;
	}

	gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const ActionDestroyAction* other = rhs->As<ActionDestroyAction>();
		return other != nullptr ? *this == *other : false;
	}

	std::string ActionDestroyAction::ToString() const
	{
		return "ActionDestroyAction"s;
	}
}
