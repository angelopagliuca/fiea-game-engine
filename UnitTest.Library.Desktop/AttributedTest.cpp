#include "pch.h"

#include <CppUnitTest.h>
#include <crtdbg.h>
#include <exception>
#include <glm/gtx/string_cast.hpp>

#include "Attributed.h"
#include "AttributedFoo.h"
#include "ToStringSpecialization.h"
#include "SizeLiteral.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}
	
	template<>
	inline ::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<vec4>(const vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<mat4>(const mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(CleanUp)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) {
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestRTTI)
		{
			AttributedFoo a;

			RTTI* rtti = &a;

			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* nf = rtti->As<Foo>();
			Assert::IsNull(nf);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);
			
			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			AttributedFoo otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Foo otherNotFoo(10);
			Assert::IsFalse(rtti->Equals(&otherNotFoo));

			Assert::AreEqual("AttributedFoo"s, r->ToString());
		}

		TEST_METHOD(TestAttributes)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"s));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"s));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"s));
			Assert::IsTrue(foo.IsAttribute("ExternalString"s));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"s));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"s));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"s));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"s));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"s));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"s));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"s));
			Assert::IsTrue(foo.IsAttribute("NestedScope"s));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"s));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"s));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"s));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"s));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"s));

			Assert::AreEqual(&foo, foo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(TestAuxiliaryAttributes)
		{
			AttributedFoo foo;

			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryInteger"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryInteger"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryInteger"));

			Datum d1 = foo.AppendAuxiliaryAttribute("AuxiliaryIntegerWithDatum");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryIntegerWithDatum"));

			Assert::ExpectException<exception>([&foo] { foo.AppendAuxiliaryAttribute("ExternalInteger"); }, L"Expected an exception but none was thrown");
		}

		TEST_METHOD(TestExternalAssignment)
		{
			AttributedFoo foo;

			Datum* datum = foo.Find("ExternalInteger");
			Assert::AreEqual(1_z, datum->Size());
			Assert::AreSame(datum->GetInteger(), foo.ExternalInteger);

			int a = 5;
			datum->Set(a);
			Assert::AreEqual(a, datum->GetInteger());
			Assert::AreEqual(a, foo.ExternalInteger);

			float b = 6.0f;
			foo["ExternalFloat"] = b;
			Assert::AreEqual(b, foo.ExternalFloat);
		}

		TEST_METHOD(TestCopySemantics)
		{
			{
				AttributedFoo foo;
				AttributedFoo anotherAttributedFoo{ foo };

				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"s));

				Assert::AreEqual(&anotherAttributedFoo, anotherAttributedFoo["this"].GetPointer()->As<AttributedFoo>());

				// Test that external storage has been updated
				int& fooData = foo["ExternalInteger"].GetInteger();
				int& anotherFooData = anotherAttributedFoo["ExternalInteger"].GetInteger();
				Assert::AreNotSame(anotherFooData, fooData);
			}
			{
				AttributedFoo foo;
				AttributedFoo anotherAttributedFoo;
				anotherAttributedFoo = foo;

				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"s));
				Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"s));

				Assert::AreEqual(&anotherAttributedFoo, anotherAttributedFoo["this"].GetPointer()->As<AttributedFoo>());

				// Test that external storage has been updated
				int& fooData = foo["ExternalInteger"].GetInteger();
				int& anotherFooData = anotherAttributedFoo["ExternalInteger"].GetInteger();
				Assert::AreNotSame(anotherFooData, fooData);
			}
		}

		TEST_METHOD(TestClear)
		{
			AttributedFoo foo;

			size_t originalSize = foo.Size();
			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::AreEqual(originalSize + 1, foo.Size());
			foo.Clear();
			Assert::AreEqual(0_z, foo.Size());
		}

		TEST_METHOD(TestAttributeAccessors)
		{
			const size_t prescribedAttributeCount = AttributedFoo::Signatures().Size() + 1; // +1 for "this"
			AttributedFoo foo;

			Assert::AreEqual(prescribedAttributeCount, foo.Attributes().Size());
			Assert::AreEqual(prescribedAttributeCount, foo.PrescribedAttributes().Size());
			Assert::AreEqual(0_z, foo.AuxiliaryAttributes().Size());

			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::AreEqual(prescribedAttributeCount + 1, foo.Attributes().Size());
			Assert::AreEqual(prescribedAttributeCount, foo.PrescribedAttributes().Size());
			Assert::AreEqual(1_z, foo.AuxiliaryAttributes().Size());
		}

		TEST_METHOD(TestMoveSemantics)
		{
			{
				AttributedFoo foo;

				size_t originalSize = foo.Size();
				Assert::AreNotEqual(0_z, originalSize);
				Assert::AreNotEqual(1_z, originalSize); // 1 for the "this" pointer (we should have more prescribed attributes)

				foo.ExternalInteger = 1;
				foo.ExternalFloat = 2.0f;
				foo.ExternalString = "Hello, World!"s;
				foo.ExternalVector = vec4(1, 2, 3, 4);
				foo.ExternalMatrix = mat4(-1);

				char helloWorld[14];
				strcpy_s(helloWorld, foo.ExternalString.c_str());
				string helloWorldString = helloWorld;
				Assert::AreEqual(helloWorldString, foo.ExternalString);

				int integerArray[] = { 1, 2, 3, 4, 5 };
				std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

				AttributedFoo anotherFoo{ std::move(foo) };
				Assert::AreEqual(originalSize, anotherFoo.Size());
				Assert::AreEqual(0_z, foo.Size());
				Assert::AreEqual(1, anotherFoo.ExternalInteger);
				Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
				Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
				Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
				Assert::IsTrue(foo.ExternalString.empty());
				Assert::AreEqual(anotherFoo.ExternalVector, vec4(1, 2, 3, 4));
				Assert::AreEqual(anotherFoo.ExternalMatrix, mat4(-1));
				Assert::IsTrue(std::equal(std::begin(anotherFoo.ExternalIntegerArray), std::end(anotherFoo.ExternalIntegerArray), std::begin(integerArray), std::end(integerArray)));
				Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());
			}
			{
				AttributedFoo foo;

				size_t originalSize = foo.Size();
				Assert::AreNotEqual(0_z, originalSize);
				Assert::AreNotEqual(1_z, originalSize); // 1 for the "this" pointer (we should have more prescribed attributes)

				foo.ExternalInteger = 1;
				foo.ExternalFloat = 2.0f;
				foo.ExternalString = "Hello, World!"s;
				foo.ExternalVector = vec4(1, 2, 3, 4);
				foo.ExternalMatrix = mat4(-1);

				char helloWorld[14];
				strcpy_s(helloWorld, foo.ExternalString.c_str());
				string helloWorldString = helloWorld;
				Assert::AreEqual(helloWorldString, foo.ExternalString);

				int integerArray[] = { 1, 2, 3, 4, 5 };
				std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

				AttributedFoo anotherFoo;
				anotherFoo = std::move(foo);
				Assert::AreEqual(originalSize, anotherFoo.Size());
				Assert::AreEqual(0_z, foo.Size());
				Assert::AreEqual(1, anotherFoo.ExternalInteger);
				Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
				Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
				Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
				Assert::IsTrue(foo.ExternalString.empty());
				Assert::AreEqual(anotherFoo.ExternalVector, vec4(1, 2, 3, 4));
				Assert::AreEqual(anotherFoo.ExternalMatrix, mat4(-1));
				Assert::IsTrue(std::equal(std::begin(anotherFoo.ExternalIntegerArray), std::end(anotherFoo.ExternalIntegerArray), std::begin(integerArray), std::end(integerArray)));
				Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());
			}
		}

		TEST_METHOD(TestNestedScopeAttributes)
		{
			AttributedFoo foo;
			Datum& nestedScopeDatum = foo.At("NestedScope"s);
			Assert::AreEqual(0_z, nestedScopeDatum.Size());

			Datum& nestedScopeArrayDatum = foo.At("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, nestedScopeArrayDatum.Size());

			const AttributedFoo& constFoo = foo;
			const Datum& nestedScopeArrayConstDatum = constFoo.At("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, nestedScopeArrayConstDatum.Size());
		}

		TEST_METHOD(TestClone)
		{
			AttributedFoo foo;
			auto clone = foo.Clone();
			auto anotherAttributedFoo = clone->As<AttributedFoo>();
			Assert::IsNotNull(anotherAttributedFoo);

			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalInteger"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalFloat"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalString"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalVector"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalMatrix"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalIntegerArray"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalFloatArray"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalStringArray"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalVectorArray"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalMatrixArray"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("NestedScope"s));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("NestedScopeArray"s));

			Assert::AreEqual(anotherAttributedFoo, (*anotherAttributedFoo)["this"s].GetPointer()->As<AttributedFoo>());

			// Test that external storage has been updated
			int& fooData = foo["ExternalInteger"].GetInteger();
			int& anotherFooData = (*anotherAttributedFoo)["ExternalInteger"].GetInteger();
			Assert::AreNotSame(anotherFooData, fooData);

			Assert::AreEqual(foo, *anotherAttributedFoo);

			delete clone;
		}

	private:
		static _CrtMemState _startMemState; // or static inline and no extra declaration
	};

	_CrtMemState AttributedTest::_startMemState;

}