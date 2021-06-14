#include "pch.h"

#include "EventMessageAttributed.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(EventMessageAttributed::TypeIdClass())
	{
	}

	EventMessageAttributed::EventMessageAttributed(const string& subType) :
		Attributed(EventMessageAttributed::TypeIdClass()), _subType(subType)
	{
	}

	const Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "SubType"s, Datum::DatumType::String, 1, offsetof(EventMessageAttributed, _subType) }
		};
	}

	const std::string& EventMessageAttributed::SubType()
	{
		return _subType;
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	gsl::owner<EventMessageAttributed*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}
}