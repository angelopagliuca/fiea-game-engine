#pragma once

#include "GameTime.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Represents the current “micro-state” of processing, including the current World, Sector, Entity, and Action.
	/// WorldState serves the role of passing data between various layers in the World, so acts like “function arguments”.
	/// </summary>
	class WorldState
	{
	public:
		WorldState() = default;
		WorldState(GameTime& gameTime);
		WorldState(WorldState&& other) noexcept = default;
		WorldState& operator=(const WorldState& other) = default;
		WorldState& operator=(WorldState&& other) noexcept = default;
		~WorldState() = default;

		/// <summary>
		/// Sets the world state's game time
		/// </summary>
		/// <param name="gameTime">GameTime to set</param>
		static void SetGameTime(GameTime& gameTime);
		/// <summary>
		/// Gets the current world state's gametime
		/// </summary>
		/// <returns></returns>
		static const GameTime& GetGameTime();
		
		static class Entity* Entity; ///< address of Entity currently being processed
		static class IAction* Action; ///< address of Action currently being processed

		static Vector<IAction*> CreateQueue;
		static Vector<IAction*> DestroyQueue;

	private:
		static GameTime _gameTime;
	};
}



