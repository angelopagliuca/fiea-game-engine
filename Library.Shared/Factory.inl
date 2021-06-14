#include "Factory.h"

namespace FieaGameEngine
{
	template<typename T>
	inline const Factory<T>* const Factory<T>::Find(const std::string& className)
	{
		auto it = _factories.Find(className);
		return it != _factories.end() ? it->second : nullptr;
	}

	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& className)
	{
		auto it = _factories.Find(className);
		return it != _factories.end() ? it->second->Create() : nullptr;
	}

	template<typename T>
	inline static void Factory<T>::Resize(size_t bucketSize)
	{
		_factories.Resize(bucketSize);
	}

	template<typename T>
	inline size_t Factory<T>::Size()
	{
		return _factories.Size();
	}

	template<typename T>
	inline bool Factory<T>::IsEmpty()
	{
		return _factories.IsEmpty();
	}
	
	template<typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		auto [it, wasInserted] = _factories.Insert(std::make_pair(factory.ClassName(), &factory));
		if (!wasInserted)
		{
			throw std::runtime_error("Factory Not Added, possibly already instantiated?");
		}
	}

	template<typename T>
	inline void Factory<T>::Remove(const Factory& factory)
	{
		_factories.Remove(factory.ClassName());
	}
}
