#pragma once

#include "RTTI.h"

namespace UnitTests
{
	class Bar : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI);
	};

	RTTI_DEFINITIONS(Bar)
}
