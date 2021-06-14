#pragma once

#include "IAction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionEvent is an action class that creates an attributed event, assigns its subtype,
	/// and copies all auxiliary parameters into the event and queues the event with the given delay.
	/// </summary>
	class ActionEvent final : public IAction
	{
	public:
		/// <summary>
		/// Constructor for ActionEvents that passes down its IdType up the hierarchy
		/// </summary>
		ActionEvent();
		ActionEvent(const ActionEvent& other) = default;
		ActionEvent(ActionEvent && other) noexcept = default;
		ActionEvent& operator=(const ActionEvent & other) = default;
		ActionEvent& operator=(ActionEvent && other) noexcept = default;
		~ActionEvent() = default;

		/// <summary>
		/// Creates an attributed event, assigns its subtype, copys all auxiliary parameters
		/// into the event and queues the event with the given delay.
		/// </summary>
		/// <param name="worldState"></param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// ActionEvent Signatures that are its prescribed attributes.
		/// </summary>
		/// <returns>the vector of signatures</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Delay member getter function
		/// </summary>
		/// <returns>delay</returns>
		const unsigned int Delay();
		/// <summary>
		/// Setter function for delay member
		/// </summary>
		/// <param name="delay">delay to set</param>
		void SetDelay(unsigned int delay);

		/// <summary>
		/// Getter function for SubType
		/// </summary>
		/// <returns>SubType</returns>
		const std::string& SubType();
		/// <summary>
		/// Setter function for SubType
		/// </summary>
		/// <param name="subType">subtype to set</param>
		void SetSubType(const std::string& subType);

	private:
		std::string _subType;
		unsigned int _delay{ 0U };
	};

	ConcreteFactory(ActionEvent, Scope)
}
