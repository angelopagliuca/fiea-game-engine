#pragma once

#include "IAction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action List is a container for Actions
	/// </summary>
	class ActionList : public IAction
	{
		RTTI_DECLARATIONS(ActionList, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionList();
		/// <summary>
		/// Constructs an ActionList with this name
		/// </summary>
		/// <param name="name">name of actionlist to set to</param>
		ActionList(const std::string& name);
		ActionList(const ActionList& other) = default;
		ActionList(ActionList&& other) noexcept = default;
		ActionList& operator=(const ActionList& other) = default;
		ActionList& operator=(ActionList&& other) noexcept = default;
		~ActionList() = default;

		/// <summary>
		/// Returns the datum of all actions in the action list
		/// </summary>
		/// <returns>datum of actions in the action list</returns>
		Datum& Actions() const;
		/// <summary>
		/// Takes an action class name and its instance name, and uses the action factory to make a new
		/// object of the given type, adopts the action into the action list, and returns the address of the new action.
		/// </summary>
		/// <param name="factory">factory to use Create with</param>
		/// <param name="name">name of the action</param>
		/// <returns>address of the new action</returns>
		IAction* CreateAction(const std::string& factory, const std::string& name);

		/// <summary>
		/// Updates all Actions in ActionList
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionList*> Clone() const override;
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

	protected:
		ActionList(RTTI::IdType type);

	private:
		const static int actionsIndex{ 2 };
	};

	ConcreteFactory(ActionList, Scope)
}

