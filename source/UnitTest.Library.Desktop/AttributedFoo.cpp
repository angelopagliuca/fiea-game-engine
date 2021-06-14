#include "pch.h"

#include "AttributedFoo.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger"s, Datum::DatumType::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat"s, Datum::DatumType::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString"s, Datum::DatumType::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector"s, Datum::DatumType::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix"s, Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray"s, Datum::DatumType::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray"s, Datum::DatumType::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray"s, Datum::DatumType::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray"s, Datum::DatumType::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray"s, Datum::DatumType::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", Datum::DatumType::Table, 0, 0 },
			{ "NestedScopeArray", Datum::DatumType::Table, ArraySize, 0 },
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const AttributedFoo* other = rhs->As<AttributedFoo>();
		return other != nullptr ? *this == *other : false;
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}


}
