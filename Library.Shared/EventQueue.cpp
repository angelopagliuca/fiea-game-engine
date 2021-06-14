#include "pch.h"

#include "EventQueue.h"

namespace FieaGameEngine
{
	bool EventQueue::QueueEntry::IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const
	{
		return currentTime - _timeEnqueued > _delay;
	}

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> e, GameTime gameTime, std::chrono::milliseconds delay)
	{
		QueueEntry entry{ std::move(e), gameTime.CurrentTime(), delay};
		_events.PushBack(entry);
	}

	void EventQueue::Send(std::shared_ptr<EventPublisher> e) const
	{
		e->Deliver();
	}

	void EventQueue::Update(GameTime& gameTime)
	{
		auto isExpiredLambda = [&gameTime](const QueueEntry& entry) { return !entry.IsExpired(gameTime.CurrentTime()); };

		Vector<QueueEntry>::Iterator bound;
		bound = std::partition(_events.begin(), _events.end(), isExpiredLambda);

		for (Vector<QueueEntry>::Iterator it=bound; it != _events.end(); ++it)
		{
			it->_event->Deliver();
		}

		_events.Remove(bound, _events.end());
	}

	void EventQueue::Clear()
	{
		_events.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return _events.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		return _events.Size();
	}
}
