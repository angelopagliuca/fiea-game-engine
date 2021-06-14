#pragma once

#include "IAction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionIncrement takes in a Target and increments the target by number of Steps.
	/// Target must be an integer and the default step is 1
	/// </summary>
	class ActionIncrement final : public IAction
	{
		RTTI_DECLARATIONS(ActionIncrement, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionIncrement();
		ActionIncrement(const ActionIncrement& other) = default;
		ActionIncrement(ActionIncrement&& other) noexcept = default;
		ActionIncrement& operator=(const ActionIncrement& other) = default;
		ActionIncrement& operator=(ActionIncrement&& other) noexcept = default;
		~ActionIncrement() = default;

		/// <summary>
		/// Gets the name of target for incrementation
		/// </summary>
		/// <returns>target to increment</returns>
		const std::string& Target() const;
		/// <summary>
		/// Sets the target of incrementation by passing in a string name
		/// </summary>
		/// <param name="target">target for increment</param>
		void SetTarget(const std::string& target);

		/// <summary>
		/// Gets the amount to increment by
		/// </summary>
		/// <returns>amount to increment by</returns>
		int Step() const;
		/// <summary>
		/// Sets the amount to increment the target by
		/// </summary>
		/// <param name="step">amount to increment</param>
		void SetStep(int step);

		/// <summary>
		/// Updates all Actions in ActionIncrement
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionIncrement*> Clone() const override;
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
		int _step = 1;
	};

	ConcreteFactory(ActionIncrement, Scope)
}

