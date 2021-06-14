#pragma once

#include "IAction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Create Action create all actions added to the worldstate
	/// create queue after the root action has gone through and updated
	/// every action.
	/// </summary>
	class ActionCreateAction final : public IAction
	{
		RTTI_DECLARATIONS(ActionCreateAction, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionCreateAction();
		ActionCreateAction(const ActionCreateAction& other) = default;
		ActionCreateAction(ActionCreateAction&& other) noexcept = default;
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;
		ActionCreateAction& operator=(ActionCreateAction&& other) noexcept = default;
		~ActionCreateAction() = default;

		/// <summary>
		/// Updates all Actions in ActionCreateAction
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionCreateAction*> Clone() const override;
		/// <summary>
		/// Equals function of the action (virtual)
		/// </summary>
		/// <param name="rhs">other action to compare</param>
		/// <returns>is other equal</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// ToString of the action
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override;

		/// <summary>
		/// Signatures of the Action Type
		/// </summary>
		/// <returns>vector of all signatures of the action</returns>
		static const Vector<Signature> Signatures();

	private:
		
	};

	ConcreteFactory(ActionCreateAction, Scope)
}

