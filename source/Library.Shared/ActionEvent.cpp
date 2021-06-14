#include "pch.h"

#include "ActionEvent.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	ActionEvent::ActionEvent() :
		IAction(ActionEvent::TypeIdClass())
	{	
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		worldState;
	}

	const Vector<Signature> ActionEvent::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionEvent, _name) },
			{ "SubType"s, Datum::DatumType::String, 1, offsetof(ActionEvent, _subType) },
			{ "Delay"s, Datum::DatumType::Integer, 1, offsetof(ActionEvent, _delay) }
		};
	}

	const unsigned int ActionEvent::Delay()
	{
		return _delay;
	}
	void ActionEvent::SetDelay(unsigned int delay)
	{
		_delay = delay;
	}

	const std::string& ActionEvent::SubType()
	{
		return _subType;
	}
	void ActionEvent::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}
}