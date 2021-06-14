#include "Event.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>);

	template<typename T>
	inline Event<T>::Event(const T& message) :
		EventPublisher(_subscribers), _message(message)
	{
	}

	template<typename T>
	inline Event<T>::Event(T&& message) :
		EventPublisher(_subscribers), _message(std::forward<T>(message))
	{
	}

	template<typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		_subscribers.PushBack(&subscriber);
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		_subscribers.Remove(&subscriber);
		_subscribers.ShrinkToFit();
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
		_subscribers.ShrinkToFit();
	}

	template<typename T>
	inline const T& FieaGameEngine::Event<T>::Message() const
	{
		return _message;
	}
	template<typename T>
	inline const Vector<EventSubscriber*>& Event<T>::Subscribers()
	{
		return _subscribers;
	}
}