#include "pch.h"

#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& subscribers) :
		_subscribers(&subscribers)
	{
	}

	void EventPublisher::Deliver() const
	{
		for (EventSubscriber* subscriber : *_subscribers)
		{
			subscriber->Notify(*this);
		}
	}
}
