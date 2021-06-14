#include "pch.h"

#include "WorldState.h"

namespace FieaGameEngine
{
	WorldState::WorldState(GameTime& gameTime)
	{
		_gameTime = gameTime;
	}
	void WorldState::SetGameTime(GameTime& gameTime)
	{
		_gameTime = gameTime;
	}
	const GameTime& WorldState::GetGameTime()
	{
		return _gameTime;
	}

	Entity* WorldState::Entity = nullptr;
	IAction* WorldState::Action = nullptr;
	GameTime WorldState::_gameTime;
}
