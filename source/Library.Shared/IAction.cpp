#include "pch.h"

#include "IAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(IAction);

	const std::string& IAction::Name() const
	{
		return _name;
	}

	void IAction::SetName(const std::string& name)
	{
		_name = name;
	}

	IAction::IAction(RTTI::IdType type) :
		Attributed{ type }
	{
	}
}