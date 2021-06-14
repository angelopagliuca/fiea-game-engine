#pragma once

#include "RTTI.h"
#include "EventSubscriber.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Event Publisher class used as a base for all events, this is the class in charge of 
	/// delivering the payloads of the events by calling their notify methods.
	/// </summary>
	class EventPublisher : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		/// <summary>
		/// Takes a list (vector) of subscribers to construct per event.
		/// </summary>
		EventPublisher(const Vector<struct EventSubscriber*>& subscribers);
		EventPublisher() = delete;
		EventPublisher(const EventPublisher& other) = default;
		EventPublisher(EventPublisher&& other) noexcept = default;
		EventPublisher& operator=(const EventPublisher& other) = default;
		EventPublisher& operator=(EventPublisher&& other) noexcept = default;
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Delivers all the subscribers of the event by Notifying
		/// all subscribers of this event.
		/// </summary>
		void Deliver() const;

	private:
		const Vector<struct EventSubscriber*>* _subscribers;
	};
}
