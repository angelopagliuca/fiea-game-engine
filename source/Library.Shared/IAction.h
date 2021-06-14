#pragma once

#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	/// <summary>
	/// IAction is the interface for all actions. It a pure virtual class and thus needs to be
	/// implemented through a base class first. It only holds a Name as its attribute
	/// </summary>
	class IAction : public Attributed
	{
		RTTI_DECLARATIONS(IAction, Attributed)

	public:
		virtual ~IAction() = default;

		/// <summary>
		/// Pure Virtual Function. Updates all Actions
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) = 0;

		/// <summary>
		/// Name of the Action
		/// </summary>
		/// <returns>Name</returns>
		const std::string& Name() const;

		/// <summary>
		/// Sets the name of the Action
		/// </summary>
		/// <param name="name">name to set</param>
		void SetName(const std::string& name);

	protected:
		IAction(RTTI::IdType type);

		std::string _name;
	};
}

