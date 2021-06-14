#include "pch.h"

#include "ActionIf.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIf);

	ActionIf::ActionIf() :
		IAction(ActionIf::TypeIdClass())
	{
		assert(_orderVector.At(trueClauseIndex)->first == "Then");
		assert(_orderVector.At(trueClauseIndex)->second.Type() == Datum::DatumType::Table);

		assert(_orderVector.At(falseClauseIndex)->first == "Else");
		assert(_orderVector.At(falseClauseIndex)->second.Type() == Datum::DatumType::Table);
	}

	const Vector<Signature> ActionIf::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumType::String, 1, offsetof(ActionIf, _name) },
			{ "Condition"s, Datum::DatumType::String, 1, offsetof(ActionIf, _target) },
			{ "Then"s, Datum::DatumType::Table, 0, 0 },
			{ "Else"s, Datum::DatumType::Table, 0, 0 },
		};
	}

	const bool ActionIf::Condition() const
	{
		return _condition;
	}

	void ActionIf::SetCondition(bool condition)
	{
		_condition = condition;
	}

	void ActionIf::Update(WorldState& worldState)
	{
		worldState.Action = this;

		size_t clauseIndex;

		if (!_target.empty())
		{
			Datum* datum = Search(_target);
			if (datum == nullptr) throw std::runtime_error("ActionIf Update - Target Does Not Exist");
			clauseIndex = datum->GetInteger() ? trueClauseIndex : falseClauseIndex;
		}
		else
		{
			clauseIndex = _condition ? trueClauseIndex : falseClauseIndex;
		}

		Datum& datum = _orderVector[clauseIndex]->second;
		Scope& scope = datum.GetScope();
		assert(scope.Is("ActionList"));
		ActionList* actionList = static_cast<ActionList*>(&scope);
		actionList->Update(worldState);
	}

	gsl::owner<ActionIf*> ActionIf::Clone() const
	{
		return new ActionIf(*this);
	}

	bool ActionIf::Equals(const RTTI* rhs) const
	{
		const ActionIf* other = rhs->As<ActionIf>();
		return other != nullptr ? *this == *other : false;
	}

	std::string ActionIf::ToString() const
	{
		return "ActionIf"s;
	}
}
