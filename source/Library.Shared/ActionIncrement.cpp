#include "pch.h"

#include "ActionIncrement.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);

	ActionIncrement::ActionIncrement() :
		IAction(ActionIncrement::TypeIdClass())
	{
	}

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionIncrement, _name) },
			{ "Target"s, Datum::DatumType::String, 1, offsetof(ActionIncrement, _target) },
			{ "Step"s, Datum::DatumType::Integer, 1, offsetof(ActionIncrement, _step) },
		};
	}

	const std::string& ActionIncrement::Target() const
	{
		return _target;
	}

	void ActionIncrement::SetTarget(const std::string& target)
	{
		_target = target;
	}

	int ActionIncrement::Step() const
	{
		return _step;
	}

	void ActionIncrement::SetStep(int step)
	{
		_step = step;
	}

	void ActionIncrement::Update(WorldState& worldState)
	{
		worldState.Action = this;

		Datum* datum = Search(_target);
		if (datum == nullptr) throw std::runtime_error("ActionIncrement Update - Target Does Not Exist");
		switch (datum->Type())
		{
			case Datum::DatumType::Integer:
				datum->GetInteger() += static_cast<int>(_step);
				break;
			default:
				throw std::runtime_error("ActionIncrement Update - Target Cannot Be Incremented");
		} 
	}

	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	bool ActionIncrement::Equals(const RTTI* rhs) const
	{
		const ActionIncrement* other = rhs->As<ActionIncrement>();
		return other != nullptr ? *this == *other : false;
	}

	std::string ActionIncrement::ToString() const
	{
		return "ActionIncrement"s;
	}
}
