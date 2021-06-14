#include "pch.h"

#include "ReactionAttributed.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() :
		Reaction(ReactionAttributed::TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const std::string& name, const std::string& subType) :
		Reaction(ReactionAttributed::TypeIdClass()), _subType{ subType }
	{
		SetName(name);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& other) :
		Reaction{ other }, _subType { other._subType }
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}
	
	ReactionAttributed::ReactionAttributed(ReactionAttributed&& other) noexcept :
		Reaction{ move(other) }, _subType{ move(other._subType) }
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed& ReactionAttributed::operator=(const ReactionAttributed& other)
	{
		if (this != &other)
		{
			Reaction::operator=(other);
			_subType = other._subType;
		}
		return *this;
	}

	ReactionAttributed& ReactionAttributed::operator=(ReactionAttributed&& other) noexcept
	{
		if (this != &other)
		{
			Reaction::operator=(std::move(other));
			_subType = move(other._subType);
		}
		return *this;
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	const Vector<Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ReactionAttributed, _name) },
			{ "Actions"s, Datum::DatumType::Table, 0, 0 },
			{ "SubType"s, Datum::DatumType::String, 1, offsetof(ReactionAttributed, _subType) }
		};
	}

	void ReactionAttributed::Notify(const EventPublisher&)
	{
	}

	const std::string& ReactionAttributed::SubType()
	{
		return _subType;
	}
	void ReactionAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}
	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}
}