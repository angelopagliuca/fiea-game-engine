#pragma once

#include "IAction.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionIf takes a target and uses its value as a condition to run all the actions
	/// in its Then and Else block of Actions. Condition, Then, and Else block are required,
	/// even if they are empty.
	/// </summary>
	class ActionIf final : public IAction
	{
		RTTI_DECLARATIONS(ActionIf, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionIf();
		ActionIf(const ActionIf& other) = default;
		ActionIf(ActionIf&& other) noexcept = default;
		ActionIf& operator=(const ActionIf& other) = default;
		ActionIf& operator=(ActionIf&& other) noexcept = default;
		~ActionIf() = default;

		/// <summary>
		/// Condition of the ActionIf
		/// </summary>
		/// <returns>if the condition is true or false</returns>
		const bool Condition() const;
		/// <summary>
		/// Sets the condition of the ActionIf
		/// </summary>
		/// <param name="condition">bool condition to set</param>
		void SetCondition(bool condition);

		/// <summary>
		/// Updates all Actions in ActionIf
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionIf*> Clone() const override;
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
		std::string _target;
		bool _condition{ false };

		const static int trueClauseIndex{ 3 };
		const static int falseClauseIndex{ 4 };

	};

	ConcreteFactory(ActionIf, Scope)
}

