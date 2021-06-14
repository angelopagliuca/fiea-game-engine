#include "pch.h"

#include "Avatar.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(Avatar);

	Avatar::Avatar() :
		Entity(Avatar::TypeIdClass())
	{
	}

	const Vector<Signature> Avatar::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(Avatar, _name) },
			{ "Children"s, Datum::DatumType::Table, 0, 0 },
			{ "Actions"s, Datum::DatumType::Table, 0, 0 },
			{ "HitPoints"s, Datum::DatumType::Integer, 1, offsetof(Avatar, _hitPoints) }
		};
	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}

	bool Avatar::Equals(const RTTI* rhs) const
	{
		const Avatar* other = rhs->As<Avatar>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Avatar::ToString() const
	{
		return "Avatar"s;
	}
}
