#pragma once

#include "Entity.h"
#include "Factory.h"

namespace UnitTests
{
	class Avatar final : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Avatar, FieaGameEngine::Entity)

	public:
		Avatar();
		Avatar(const Avatar&) = default;
		Avatar(Avatar&&) noexcept = default;
		Avatar& operator=(const Avatar&) = default;
		Avatar& operator=(Avatar&&) noexcept = default;
		~Avatar() = default;

		gsl::owner<Avatar*> Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	
	private:
		int _hitPoints = 100;
	};

	ConcreteFactory(Avatar, FieaGameEngine::Scope)
}

