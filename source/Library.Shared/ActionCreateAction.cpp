#include "pch.h"

#include "ActionCreateAction.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction() :
		IAction(ActionCreateAction::TypeIdClass())
	{
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionCreateAction, _name) },
		};
	}


	void ActionCreateAction::Update(WorldState& worldState)
	{
		worldState.Action = this;
	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const ActionCreateAction* other = rhs->As<ActionCreateAction>();
		return other != nullptr ? *this == *other : false;
	}

	std::string ActionCreateAction::ToString() const
	{
		return "ActionCreateAction"s;
	}
}
