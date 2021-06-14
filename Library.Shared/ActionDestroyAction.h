#pragma once

#include "IAction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Destroy Action destroys all actions added to the worldstate
	/// destroy queue at after the root action has gone through and updated
	/// every action.
	/// </summary>
	class ActionDestroyAction final : public IAction
	{
		RTTI_DECLARATIONS(ActionDestroyAction, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionDestroyAction();
		ActionDestroyAction(const ActionDestroyAction& other) = default;
		ActionDestroyAction(ActionDestroyAction&& other) noexcept = default;
		ActionDestroyAction& operator=(const ActionDestroyAction& other) = default;
		ActionDestroyAction& operator=(ActionDestroyAction&& other) noexcept = default;
		~ActionDestroyAction() = default;

		/// <summary>
		/// Updates all Actions in ActionDestroyAction
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionDestroyAction*> Clone() const override;
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

	ConcreteFactory(ActionDestroyAction, Scope)
}

