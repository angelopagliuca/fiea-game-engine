#pragma once

#include <chrono>

#include "Vector.h"
#include "EventPublisher.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	using namespace std::chrono_literals;

	/// <summary>
	/// Event Queue is the class that acts as the container for all active events, it holds events and delivers them
	/// based on their delay, they can be can be delivered immediately, or queued for future delivery.
	/// </summary>
	class EventQueue final
	{
	public:
		EventQueue() = default;
		EventQueue(const EventQueue& other) = delete;
		EventQueue(EventQueue&& other) noexcept = default;
		EventQueue& operator=(const EventQueue& other) = delete;
		EventQueue& operator=(EventQueue&& other) noexcept = default;
		~EventQueue() = default;

		/// <summary>
		/// Given the address of an EventPublisher, a GameTime, and an optional delay time, enqueues the event.
		/// Delivers it on Update once delay time has passed.
		/// </summary>
		/// <param name="e">event to put in the queue</param>
		/// <param name="gameTime">used to retrieve the current time</param>
		/// <param name="delay">optional delay to Deliver after time expiries</param>
		void Enqueue(std::shared_ptr<EventPublisher> e, GameTime gameTime, std::chrono::milliseconds delay = 0s);
		/// <summary>
		/// Given the address of an EventPublisher, sends the event immediately.
		/// </summary>
		/// <param name="e">event to send immediately</param>
		void Send(std::shared_ptr<EventPublisher> e) const;
		/// <summary>
		/// Given the a GameTime, publish any queued events that have expired.
		/// </summary>
		/// <param name="gameTime">current gameTime for timing</param>
		void Update(GameTime& gameTime);

		/// <summary>
		/// Clear the event queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// returns a boolean indicating the emptiness of the queue.
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const;
		/// <summary>
		/// returns the number of events in the queue.
		/// </summary>
		/// <returns></returns>
		size_t Size() const;

	private:
		struct QueueEntry
		{
			std::shared_ptr<EventPublisher> _event;
			std::chrono::high_resolution_clock::time_point _timeEnqueued;
			std::chrono::milliseconds _delay;

			bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const;
		};

		Vector<QueueEntry> _events;
	};
}

