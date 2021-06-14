#include "pch.h"

#include "ActionExpression.h"

namespace FieaGameEngine
{
	using namespace std;
	using namespace std::string_literals;

	RTTI_DEFINITIONS(ActionExpression)

	ActionExpression::ActionExpression() :
		IAction(ActionExpression::TypeIdClass())
	{
	}

	const Vector<Signature> ActionExpression::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(ActionExpression, _name) },
			{ "Expression", Datum::DatumType::String, 1, offsetof(ActionExpression, _expression) }
		};
	}

	void ActionExpression::SetExpression(const std::string& expression)
	{
		_expression = expression;
	}

	const std::string& ActionExpression::Expression() const
	{
		return _expression;
	}

	int ActionExpression::Result() const
	{
		return _result;
	}

	typename gsl::owner<ActionExpression*> ActionExpression::Clone() const
	{
		return new ActionExpression(*this);
	}

	void ActionExpression::Update(WorldState&)
	{
		if (_expression == "")
		{
			throw runtime_error("Empty expression");
		}

		Vector<std::string> tokenVector;
		Stack<std::pair<std::string, int>> operatorStack;
		Vector<std::string> outputQueue;
		Stack<int> valueStack;

		// Separate Tokens by spaces into Token Vector
		string expressionToken;
		for (const char& c : _expression)
		{
			if (c == ' ')
			{
				tokenVector.PushBack(expressionToken);
				expressionToken = ""s;
			}
			else
			{
				expressionToken += c;
			}
		}
		tokenVector.PushBack(expressionToken);

		// The Shunting Yard Algorithm
		HashMap<string, int>::ConstIterator operatorMapIterator;
		HashMap<string, int>::ConstIterator operatorMapEnd = _operators.end();
		HashMap<string, int>::ConstIterator finalOperatorVectorIterator;
		HashMap<string, int>::ConstIterator finalOperatorVectorEnd = _finalOperators.end();

		string operatorString = "";
		for (const string& token : tokenVector)
		{
			operatorMapIterator = _operators.Find(token);
			finalOperatorVectorIterator = _finalOperators.Find(token);
			if (operatorMapIterator != operatorMapEnd)
			{
				while ((!operatorStack.IsEmpty()) && (operatorMapIterator->second <= operatorStack.Top().second))
				{
					outputQueue.PushBack(operatorStack.Top().first);
					operatorStack.Pop();
				}

				operatorStack.Push(*operatorMapIterator);
			}
			else if (finalOperatorVectorIterator != finalOperatorVectorEnd)
			{
				operatorString = token;
			}
			else if (token == "(")
			{
				operatorStack.Push(make_pair("(", -1));
			}
			else if (token == ")")
			{
				while (operatorStack.Top().first != "(")
				{
					outputQueue.PushBack(operatorStack.Top().first);
					operatorStack.Pop();

					if (operatorStack.IsEmpty())
					{
						throw runtime_error("Left Parentheses Not Found");
					}
				}

				operatorStack.Pop();
			}
			else
			{
				outputQueue.PushBack(token);
			}
		}

		while (!operatorStack.IsEmpty())
		{
			outputQueue.PushBack(operatorStack.Top().first);
			operatorStack.Pop();
		}

		if (operatorString != "")
		{
			outputQueue.PushBack(operatorString);
		}

		tokenVector.Clear();

		// Reverse Polish
		Datum* foundDatum = nullptr;
		Datum* cachedDatum = nullptr;
		ExpressionFunction func = nullptr;
		ExpressionAssignFunction assignFunc = nullptr;
		for (string token : outputQueue)
		{
			operatorMapIterator = _operators.Find(token);
			finalOperatorVectorIterator = _finalOperators.Find(token);
			if (operatorMapIterator != operatorMapEnd)
			{
				func = _expressionFunctionMap.Find(token)->second;
				assert(func != nullptr);

				int rhs = valueStack.Top();
				valueStack.Pop();
				int lhs = valueStack.Top();
				valueStack.Pop();

				valueStack.Push((*this.*func)(lhs, rhs));
			}
			else if (finalOperatorVectorIterator != finalOperatorVectorEnd)
			{
				if (token == "==" or token == "!=")
				{
					func = _expressionFunctionMap.Find(token)->second;
					assert(func != nullptr);

					int rhs = valueStack.Top();
					valueStack.Pop();
					int lhs = valueStack.Top();
					valueStack.Pop();

					valueStack.Push((*this.*func)(lhs, rhs));
				}
				else
				{
					assignFunc = _expressionAssignFunctionMap.Find(token)->second;
					assert(assignFunc != nullptr);

					int rhs = valueStack.Top();
					valueStack.Pop();
					int& lhs = cachedDatum->GetInteger();
					valueStack.Pop();


					valueStack.Push((*this.*assignFunc)(lhs, rhs));
				}
			}
			else
			{
				foundDatum = Search(token);

				if (foundDatum == nullptr)
				{
					throw runtime_error("Attribute was not Found, make sure it's correct.");
				}

				if (cachedDatum == nullptr)
				{
					cachedDatum = foundDatum;
				}

				valueStack.Push(foundDatum->GetInteger());
			}
		}

		_result = valueStack.Top();
	}

	int ActionExpression::Add(int lhs, int rhs)
	{
		return (lhs + rhs);
	}

	int ActionExpression::Subtract(int lhs, int rhs)
	{
		return (lhs - rhs);
	}

	int ActionExpression::Multiply(int lhs, int rhs)
	{
		return (lhs * rhs);
	}

	int ActionExpression::Divide(int lhs, int rhs)
	{
		return (lhs / rhs);
	}

	int ActionExpression::Mod(int lhs, int rhs)
	{
		return (lhs % rhs);
	}

	int ActionExpression::Equals(int lhs, int rhs)
	{
		return (lhs == rhs);
	}

	int ActionExpression::NotEquals(int lhs, int rhs)
	{
		return (lhs != rhs);
	}

	int ActionExpression::Assign(int& lhs, int rhs)
	{
		lhs = rhs;
		return lhs;
	}

	int ActionExpression::AddAssign(int& lhs, int rhs)
	{
		lhs += rhs;
		return lhs;
	}

	int ActionExpression::SubtractAssign(int& lhs, int rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	int ActionExpression::MultiplyAssign(int& lhs, int rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	int ActionExpression::DivideAssign(int& lhs, int rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	int ActionExpression::ModAssign(int& lhs, int rhs)
	{
		lhs = lhs % rhs;
		return lhs;
	}
}
