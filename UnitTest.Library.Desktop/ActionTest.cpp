#include "pch.h"

#include <crtdbg.h>
#include <CppUnitTest.h>
#include <algorithm>

#include "ToStringSpecialization.h"
#include "Entity.h"
#include "Avatar.h"
#include "GameTime.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "WorldState.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionIf.h"
#include "ActionExpression.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	wstring ToString<ActionList>(const ActionList& t) 
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionIf>(const ActionIf& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITest)
		{
			Entity entity;

			RTTI* rtti = &entity;

			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Entity"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Entity::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Entity::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			Entity* e = rtti->As<Entity>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&entity, e);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&entity), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(Entity::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			Entity otherEntity;
			Assert::IsTrue(rtti->Equals(&otherEntity));

			Foo otherFoo(10);
			Assert::IsFalse(rtti->Equals(&otherFoo));

			Assert::AreEqual("Entity"s, r->ToString());
		}

		TEST_METHOD(TestActionIncrement)
		{
			ActionIncrement increment;
			Datum& a = increment.AppendAuxiliaryAttribute("A"s);
			a = 0;
			increment.SetTarget("A"s);
			Assert::AreEqual("A"s, increment.Target());
			Assert::AreEqual(1, increment.Step()); // Default Step = 1.0f

			GameTime gameTime;
			WorldState worldState(gameTime);
			increment.Update(worldState);
			Assert::AreEqual(1, a.GetInteger());

			increment.Update(worldState);
			Assert::AreEqual(2, a.GetInteger());

			increment.SetStep(2);
			Assert::AreEqual(2, increment.Step());
			increment.Update(worldState);
			Assert::AreEqual(4, a.GetInteger());

			// Target does not exist
			increment.SetTarget("B");
			Assert::ExpectException<exception>([&increment, &worldState] { increment.Update(worldState); });

			// Target exists but cannot be incremented
			Datum& b = increment.AppendAuxiliaryAttribute("B"s);
			b = "This cannot be incremented"s;
			Assert::ExpectException<exception>([&increment, &worldState] { increment.Update(worldState); });
		}

		TEST_METHOD(TestActionIf)
		{
			GameTime gameTime;
			WorldState worldState(gameTime);
			ActionIf actionIf;
			Datum& a = actionIf.AppendAuxiliaryAttribute("A"s);
			a = 0;
			Datum& b = actionIf.AppendAuxiliaryAttribute("B"s);
			b = 0;

			ActionIncrementFactory actionIncrementFactory;
			ActionList trueActionList("Test True Clause Action"s);
			IAction* trueClause = trueActionList.CreateAction(ActionIncrement::TypeName(), "True Clause"s);
			Assert::IsNotNull(trueClause);
			ActionIncrement* aIncrement = trueClause->As<ActionIncrement>();
			Assert::IsNotNull(aIncrement);
			aIncrement->SetTarget("A"s);
			actionIf.Adopt(trueActionList, "Then");

			ActionList falseActionList("Test True Clause Action"s);
			IAction* falseClause = falseActionList.CreateAction(ActionIncrement::TypeName(), "False Clause"s);
			Assert::IsNotNull(falseClause);
			ActionIncrement* bIncrement = falseClause->As<ActionIncrement>();
			Assert::IsNotNull(bIncrement);
			bIncrement->SetTarget("B"s);
			actionIf.Adopt(falseActionList, "Else");

			{
				// True clause
				actionIf.SetCondition(true);
				actionIf.Update(worldState);
				Assert::AreEqual(1, a.GetInteger());
				Assert::AreEqual(0, b.GetInteger());

				actionIf.Update(worldState);
				Assert::AreEqual(2, a.GetInteger());
				Assert::AreEqual(0, b.GetInteger());

				actionIf.Update(worldState);
				Assert::AreEqual(3, a.GetInteger());
				Assert::AreEqual(0, b.GetInteger());
			}

			{
				// False clause
				actionIf.SetCondition(false);
				actionIf.Update(worldState);
				Assert::AreEqual(1, b.GetInteger());
				Assert::AreEqual(3, a.GetInteger());

				actionIf.Update(worldState);
				Assert::AreEqual(2, b.GetInteger());
				Assert::AreEqual(3, a.GetInteger());

				actionIf.Update(worldState);
				Assert::AreEqual(3, b.GetInteger());
				Assert::AreEqual(3, a.GetInteger());
			}

			bIncrement->SetTarget("C"s);
			Assert::ExpectException<exception>([&actionIf, &worldState] {actionIf.Update(worldState); });

			auto clonedScope = actionIf.Clone();
			Assert::IsNotNull(clonedScope);
			auto clone = clonedScope->As<ActionIf>();
			Assert::IsNotNull(clone);
			Assert::AreEqual(actionIf, *clone);
			delete clone;
		}

		TEST_METHOD(TestActionExpression)
		{
			GameTime gameTime;
			WorldState worldState(gameTime);

			ActionExpression actionExpression;
			Datum* a = &actionExpression.Append("A"s);
			Datum* b = &actionExpression.Append("B"s);
			*a = 7;
			*b = 5;

			{
				actionExpression.SetExpression("A + B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(12, actionExpression.Result());
				actionExpression.SetExpression("A - B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(2, actionExpression.Result());
				actionExpression.SetExpression("A * B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(35, actionExpression.Result());
				actionExpression.SetExpression("A / B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(1, actionExpression.Result());
				actionExpression.SetExpression("A % B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(2, actionExpression.Result());
				actionExpression.SetExpression("( A + B ) + ( A - B ) + ( A * B ) + ( A / B ) + ( A % B )"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(52, actionExpression.Result());

				actionExpression.SetExpression("A == B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(false, (bool)actionExpression.Result());
				actionExpression.SetExpression("A != B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(true, (bool)actionExpression.Result());
				*a = 5;
				actionExpression.SetExpression("A == B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(true, (bool)actionExpression.Result());
				actionExpression.SetExpression("A != B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(false, (bool)actionExpression.Result());
			}

			*a = 8;
			*b = 4;
			{
				actionExpression.SetExpression("A += B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(12, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(12, a->GetInteger());
				actionExpression.SetExpression("A -= B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(8, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(8, a->GetInteger());
				actionExpression.SetExpression("A *= B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(32, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(32, a->GetInteger());
				actionExpression.SetExpression("A /= B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(8, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(8, a->GetInteger());
				actionExpression.SetExpression("A %= B"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(0, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(0, a->GetInteger());

				*a = 6;
				actionExpression.SetExpression("A = ( A + B ) + ( A - B ) + ( A * B ) + ( A / B ) + ( A % B )"s);
				actionExpression.Update(worldState);
				Assert::AreEqual(39, actionExpression.Result());
				Assert::AreEqual(a->GetInteger(), actionExpression.Result());
				Assert::AreEqual(39, a->GetInteger());
			}
		}

		TEST_METHOD(TestJsonDeserialization)
		{
			ActionIncrementFactory actionIncrementFactory;
			ActionListFactory actionListFactory;
			ActionIfFactory actionIfFactory;
			ActionExpressionFactory actionExpressionFactory;
			ActionList actionList;

			{
				// Deserialization 
				JsonTableParseHelper::SharedData sharedData(actionList);
				JsonParseCoordinator parseMaster(sharedData);
				JsonTableParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				const std::string filename = R"(Content\TestAction.json)"s;
				parseMaster.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Action"s, actionList.Name());

			auto& actionsDatum = actionList.Actions();
			Assert::AreEqual(3_z, actionsDatum.Size());
			for (size_t i = 0; i < actionsDatum.Size(); ++i)
			{
				IAction* action = actionsDatum[i].As<IAction>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			ActionList& nestedActionList = *(actionsDatum[1].As<ActionList>());
			auto& nestedActionListActionsDatum = nestedActionList.Actions();
			Assert::AreEqual(3_z, nestedActionListActionsDatum.Size());
			for (size_t i = 0; i < nestedActionListActionsDatum.Size(); ++i)
			{
				IAction* action = nestedActionListActionsDatum[i].As<IAction>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			{
				auto action = nestedActionListActionsDatum[1].As<ActionList>();
				Assert::IsNotNull(action);
				Assert::IsTrue(action->IsAuxiliaryAttribute("TestAttribute"s));
				Assert::AreEqual(100, action->Find("TestAttribute")->GetInteger());
			}

			{
				auto action = nestedActionListActionsDatum[2].As<ActionIf>();
				Assert::IsNotNull(action);
				ActionIf& actionIf = *action;
				Assert::AreEqual(false, actionIf.Condition());
				
				GameTime gameTime;
				WorldState worldState(gameTime);

				Datum* X = actionIf.Search("X"s);
				Assert::IsNotNull(X);
				Assert::AreEqual(1, X->GetInteger());

				actionIf.Update(worldState);

				Assert::AreEqual(2, X->GetInteger());
			}

			ActionList& nestedActionListExp = *(actionsDatum[2].As<ActionList>());

			Assert::IsTrue(nestedActionListExp.IsAuxiliaryAttribute("A"s));
			Assert::IsTrue(nestedActionListExp.IsAuxiliaryAttribute("B"s));
			Assert::IsTrue(nestedActionListExp.IsAuxiliaryAttribute("C"s));
			Assert::IsTrue(nestedActionListExp.IsAuxiliaryAttribute("D"s));
			Assert::AreEqual(3, nestedActionListExp.Find("A")->GetInteger());
			Assert::AreEqual(5, nestedActionListExp.Find("B")->GetInteger());
			Assert::AreEqual(3, nestedActionListExp.Find("C")->GetInteger());
			Assert::AreEqual(0, nestedActionListExp.Find("D")->GetInteger());

			auto& nestedActionListExpActionsDatum = nestedActionListExp.Actions();
			Assert::AreEqual(1_z, nestedActionListExpActionsDatum.Size());
			{
				auto action = nestedActionListExpActionsDatum[0].As<ActionExpression>();
				Assert::IsNotNull(action);
				ActionExpression& actionExpression = *action;
				Assert::AreEqual(0, actionExpression.Result());

				GameTime gameTime;
				WorldState worldState(gameTime);

				Datum* A = actionExpression.Search("A"s);
				Assert::IsNotNull(A);
				Assert::AreEqual(3, A->GetInteger());
				Datum* B = actionExpression.Search("B"s);
				Assert::IsNotNull(B);
				Assert::AreEqual(5, B->GetInteger());
				Datum* C = actionExpression.Search("C"s);
				Assert::IsNotNull(C);
				Assert::AreEqual(3, C->GetInteger());
				Datum* D = actionExpression.Search("D"s);
				Assert::IsNotNull(D);
				Assert::AreEqual(0, D->GetInteger());

				actionExpression.Update(worldState);

				Assert::AreEqual(30, D->GetInteger());
			}
		}

		TEST_METHOD(Clone)
		{
			ActionList action("Test Action"s);

			auto clone = action.Clone();
			Assert::AreNotSame(action, *clone);
			Assert::AreEqual(action, *clone);

			delete clone;
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTest::sStartMemState;
}