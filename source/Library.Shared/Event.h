#pragma once

#include "EventSubscriber.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	/// <summary>
	/// This system implements the observer pattern and allows for the publication of events to multiple subscribers.
	/// Events carry a templated data “payload” – the message of the event – and can be delivered immediately, or queued for future delivery.
	/// </summary>
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);

	public:
		/// <summary>
		/// Takes in an lvalue payload (message) to copy and carry by event.
		/// </summary>
		/// <param name="message">payload to hold per event</param>
		explicit Event(const T& message);
		/// <summary>
		/// Takes in an rvalue payload (message) to copy and carry by event.
		/// </summary>
		/// <param name="message">payload to hold per event</param>
		explicit Event(T&& message);
		Event() = delete;
		Event(const Event& other) = default;
		Event(Event&& other) noexcept = default;
		Event& operator=(const Event& other) = default;
		Event& operator=(Event&& other) noexcept = default;
		virtual ~Event() = default;

		/// <summary>
		/// (static) Given the address of an EventSubscriber, adds it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">subscriber to add for event type</param>
		static void Subscribe(EventSubscriber& subscriber);
		/// <summary>
		/// (static) Given the address of an EventSubscriber, removes it from the list of subscribers for this event type.
		/// </summary>
		/// <param name="">subscriber to remove for event type</param>
		static void Unsubscribe(EventSubscriber&);
		/// <summary>
		/// (static)  Unsubscribes all subscribers to this event type.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Returns the message object -- the payload.
		/// </summary>
		/// <returns>payload</returns>
		const T& Message() const;

		/// <summary>
		/// Subscribers of the Event
		/// </summary>
		/// <returns></returns>
		static const Vector<EventSubscriber*>& Subscribers();

	private:
		static inline Vector<EventSubscriber*> _subscribers;

		T _message;
	};
}

#include "Event.inl"