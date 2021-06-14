#pragma once

#include "IAction.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Expression is the Action class that is able to take in a string in the form of a
	/// mathematical expression and do the correct and corresponding expression using int. It does 
	/// all the computations on attributes that it must search to fulfil the expression. It must use
	/// int and returns an int. If the result is meant to be a bool you can cast it to a bool as the
	/// result will be either 1 or 0. Each Attribute must be separated by a space.
	/// </summary>
	class ActionExpression final : public IAction
	{
		RTTI_DECLARATIONS(ActionExpression, IAction)

	public:
		/// <summary>
		/// Default Constructor passes down constructor to attributed
		/// </summary>
		ActionExpression();
		ActionExpression(const ActionExpression&) = default;
		ActionExpression(ActionExpression&&) noexcept = default;
		ActionExpression& operator=(const ActionExpression&) = default;
		ActionExpression& operator=(ActionExpression&&) noexcept = default;
		~ActionExpression() = default;

		/// <summary>
		/// Sets the expression of the action to fulfil
		/// </summary>
		/// <param name="expression">new expression</param>
		void SetExpression(const std::string& expression);
		/// <summary>
		/// Expression being fulfilled by this Action
		/// </summary>
		/// <returns>The expression used by this action.</returns>
		const std::string& Expression() const;

		/// <summary>
		/// Retrieves the result of the latest expression that was performed.
		/// </summary>
		/// <returns>result of the expression.</returns>
		int Result() const;

		/// <summary>
		/// Updates all Actions in ActionExpression to fulfil the expression set
		/// </summary>
		/// <param name="worldState"></param>
		void Update(WorldState& worldState) override;

		/// <summary>
		/// Clones the Action (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ActionExpression*> Clone() const override;

		/// <summary>
		/// Signatures of the Action Type
		/// </summary>
		/// <returns>vector of all signatures of the action</returns>
		static const Vector<Signature> Signatures();

	private:
		std::string _expression;
		int _result = 0;

		int Add(int lhs, int rhs);
		int Subtract(int lhs, int rhs);
		int Multiply(int lhs, int rhs);
		int Divide(int lhs, int rhs);
		int Mod(int lhs, int rhs);
		int Equals(int lhs, int rhs);
		int NotEquals(int lhs, int rhs);

		using ExpressionFunction = int(ActionExpression::*)(int, int);
		using ExpressionFunctionMap = HashMap<std::string, ExpressionFunction>;
		const ExpressionFunctionMap _expressionFunctionMap = {
			std::make_pair("+", &ActionExpression::Add),
			std::make_pair("-", &ActionExpression::Subtract),
			std::make_pair("*", &ActionExpression::Multiply),
			std::make_pair("/", &ActionExpression::Divide),
			std::make_pair("%", &ActionExpression::Mod),
			std::make_pair("==", &ActionExpression::Equals),
			std::make_pair("!=", &ActionExpression::NotEquals)
		};

		int Assign(int& lhs, int rhs);
		int AddAssign(int& lhs, int rhs);
		int SubtractAssign(int& lhs, int rhs);
		int MultiplyAssign(int& lhs, int rhs);
		int DivideAssign(int& lhs, int rhs);
		int ModAssign(int& lhs, int rhs);

		using ExpressionAssignFunction = int(ActionExpression::*)(int&, int);
		using ExpressionAssignFunctionMap = HashMap<std::string, ExpressionAssignFunction>;
		const ExpressionAssignFunctionMap _expressionAssignFunctionMap = {
			std::make_pair("=", &ActionExpression::Assign),
			std::make_pair("+=", &ActionExpression::AddAssign),
			std::make_pair("-=", &ActionExpression::SubtractAssign),
			std::make_pair("*=", &ActionExpression::MultiplyAssign),
			std::make_pair("/=", &ActionExpression::DivideAssign),
			std::make_pair("%=", &ActionExpression::ModAssign)
		};

		inline static const HashMap<std::string, int> _operators = {
			std::make_pair("+", 0),			// Add
			std::make_pair("-", 0),			// Substract
			std::make_pair("*", 1),			// Multiply
			std::make_pair("/", 1),			// Divide	
			std::make_pair("%", 1)			// Mod
		};
		inline static const HashMap<std::string, int> _finalOperators = {
			std::make_pair("=", 0),			// Assign
			std::make_pair("+=", 0),		// AddAssign
			std::make_pair("-=", 0),		// SubstractAssign
			std::make_pair("*=", 0),		// MultiplyAssign
			std::make_pair("/=", 0),		// DivideAssign
			std::make_pair("%=", 0),		// ModAssign
			std::make_pair("==", 0),		// Equals
			std::make_pair("!=", 0)			// NotEquals
		};
	};

	ConcreteFactory(ActionExpression, Scope);
}