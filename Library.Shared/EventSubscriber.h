#pragma once

#include "EventPublisher.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract Base class for event subscribers, the most important thing for EventSubscriber
	/// is to have their Notify method implemented to actually do the work of the event.
	/// </summary>
	struct EventSubscriber
	{
		EventSubscriber() = default;
		EventSubscriber(const EventSubscriber& other) = default;
		EventSubscriber(EventSubscriber&& other) noexcept = default;
		EventSubscriber& operator=(const EventSubscriber& other) = default;
		EventSubscriber& operator=(EventSubscriber&& other) noexcept = default;
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// Pure virtual method that accepts the address of an EventPublisher. 
		/// Verifis actual the event type using RTTI. Does all the work of the payload.
		/// </summary>
		/// <param name="eventPublisher">the actual argument will be the event itself</param>
		virtual	void Notify(const class EventPublisher&) = 0;
	};
}
