#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reactions derive from ActionList and EventSubscriber, hence tie the event system to the rest of the engine.
	/// Typically, Reaction::Notify (and any Reaction derived class) will invoke ActionList::Update.
	/// Depending on the specialization of Reaction, Notify might also do additional processing
	/// e.g. to extract arguments from the event and copy them into the Scope of the invoked ActionList. 
	/// </summary>
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		Reaction() = default;
		Reaction(const Reaction&) = default;
		Reaction(Reaction&&) = default;
		Reaction& operator=(const Reaction&) = default;
		Reaction& operator=(Reaction&&) = default;
		~Reaction() = default;

	protected:
		Reaction(RTTI::IdType);
	};
}