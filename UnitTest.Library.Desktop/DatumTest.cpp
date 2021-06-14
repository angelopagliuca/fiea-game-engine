#include "pch.h"

#include <CppUnitTest.h>
#include <crtdbg.h>
#include <exception>
#include <initializer_list>
#include <glm/gtx/string_cast.hpp>

#include "Foo.h"
#include "Scope.h"
#include "Datum.h"
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
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		string string = ""s;
		for (size_t i = 0_z; i < t.Size(); ++i)
		{
			if (t.Type() != Datum::DatumType::Table) string.append(t.ToString(i));
		}
		return ToString(string);
	}

	template<>
	inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t)
	{
		return ToString(static_cast<int>(t));
	}

	template<>
	inline std::wstring ToString<vec4>(const vec4& t)
	{
		return ToString(to_string(t));
	}

	template<>
	inline std::wstring ToString<mat4>(const mat4& t)
	{
		return ToString(to_string(t));
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		return ToString(t->ToString());
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope& t)
	{
		return ToString(t.ToString());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(TestConstructor)
		{
			{
				Datum datum;
				Assert::AreEqual(Datum::DatumType::Unknown, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());

				Assert::ExpectException<exception>([&datum] { datum.Reserve(0); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(0); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Unknown); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::Integer };
				Assert::AreEqual(Datum::DatumType::Integer, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Float); }, L"Expected an exception but none was thrown");
				
				Datum scalar = int{ 10 };
				Assert::AreEqual(Datum::DatumType::Integer, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::Float); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::Float };
				Assert::AreEqual(Datum::DatumType::Float, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Integer); }, L"Expected an exception but none was thrown");
				Datum scalar = float{ 10 };
				Assert::AreEqual(Datum::DatumType::Float, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::Integer); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::Vector };
				Assert::AreEqual(Datum::DatumType::Vector, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Matrix); }, L"Expected an exception but none was thrown");
				Datum scalar = vec4{ 10 };
				Assert::AreEqual(Datum::DatumType::Vector, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::Matrix); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::Matrix };
				Assert::AreEqual(Datum::DatumType::Matrix, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Vector); }, L"Expected an exception but none was thrown");
				Datum scalar = mat4{ 10 };
				Assert::AreEqual(Datum::DatumType::Matrix, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::Vector); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::String };
				Assert::AreEqual(Datum::DatumType::String, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::Pointer); }, L"Expected an exception but none was thrown");
				Datum scalar = string{ "Hello"s };
				Assert::AreEqual(Datum::DatumType::String, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::Pointer); }, L"Expected an exception but none was thrown");
			}
			{
				Datum datum{ Datum::DatumType::Pointer };
				Assert::AreEqual(Datum::DatumType::Pointer, datum.Type());
				Assert::AreEqual(0_z, datum.Size());
				Assert::AreEqual(0_z, datum.Capacity());
				Assert::ExpectException<exception>([&datum] { datum.SetType(Datum::DatumType::String); }, L"Expected an exception but none was thrown");
				Foo fa{ 10 };
				RTTI* a = &fa;
				Datum scalar = a;
				Assert::AreEqual(Datum::DatumType::Pointer, scalar.Type());
				Assert::AreEqual(1_z, scalar.Size());
				Assert::AreEqual(1_z, scalar.Capacity());
				Assert::ExpectException<exception>([&scalar] { scalar.SetType(Datum::DatumType::String); }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestCopySemantics)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetInteger(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetInteger(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetFloat(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetFloat(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetVector(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetVector(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetMatrix(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetMatrix(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetString(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetString(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				Datum datum{ a, b, c };
				Assert::AreEqual(3_z, datum.Size());

				{
					Datum copyOfDatum{ datum };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = datum;
					Assert::AreEqual(3_z, copyOfDatum.Size());
					Assert::AreEqual(c, copyOfDatum.GetPointer(2));
					copyOfDatum = { c, b, a, b, c };
					Assert::AreEqual(5_z, copyOfDatum.Size());
					Assert::AreEqual(a, copyOfDatum.GetPointer(2));
					copyOfDatum = a;
					Assert::AreEqual(1_z, copyOfDatum.Size());
					Assert::AreEqual(1_z, copyOfDatum.Capacity());
				}
			}
		}

		TEST_METHOD(TestMoveSemantics)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ move(datum) };
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
				{
					Datum datum{ a, b, c };
					Datum copyOfDatum{ a, b };
					Assert::AreEqual(2_z, copyOfDatum.Size());
					copyOfDatum = move(datum);
					Assert::AreEqual(3_z, copyOfDatum.Size());
				}
			}
		}

		TEST_METHOD(TestComparisons)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);
				
				Datum otherTypeDatum = 10.f;
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum == 10.f; }, L"Expected an exception but none was thrown");
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);

				Datum otherTypeDatum = 10;
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum == 10; }, L"Expected an exception but none was thrown");
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);

				Datum otherTypeDatum = mat4(10.f);
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum == mat4(10.f); }, L"Expected an exception but none was thrown");
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);

				Datum otherTypeDatum = vec4(10.f);
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum == vec4(10.f); }, L"Expected an exception but none was thrown");
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);

				Foo fa{ 10 };
				RTTI* p = &fa;
				Datum otherTypeDatum = p;
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &p] { unknownDatum == p; }, L"Expected an exception but none was thrown");
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				Datum datum{ a, b };
				Datum otherDatum{ a, b };
				Assert::IsTrue(datum == otherDatum);

				otherDatum = { b, c };
				Assert::IsTrue(datum != otherDatum);

				Assert::IsTrue(datum != a);

				datum = c;
				Assert::IsTrue(datum == c);

				Datum otherTypeDatum = "String"s;
				Assert::IsTrue(datum != otherTypeDatum);

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&datum, &unknownDatum] { datum == unknownDatum; }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum == "String"s; }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestSetAndGet)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetInteger());
				Assert::AreEqual(b, datum.GetInteger(1_z));
				
				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetInteger(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetInteger(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetInteger(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetInteger(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetInteger(); }, L"Expected an exception but none was thrown");

			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetFloat());
				Assert::AreEqual(b, datum.GetFloat(1_z));

				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetFloat(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetFloat(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetFloat(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetFloat(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetFloat(); }, L"Expected an exception but none was thrown");
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetVector());
				Assert::AreEqual(b, datum.GetVector(1_z));

				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetVector(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetVector(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetVector(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetVector(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetVector(); }, L"Expected an exception but none was thrown");
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetMatrix());
				Assert::AreEqual(b, datum.GetMatrix(1_z));

				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetMatrix(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetMatrix(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetMatrix(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetMatrix(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetMatrix(); }, L"Expected an exception but none was thrown");
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetString());
				Assert::AreEqual(b, datum.GetString(1_z));

				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetString(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetString(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetString(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetString(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetString(); }, L"Expected an exception but none was thrown");
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				Datum datum = { a, b };
				Assert::AreEqual(a, datum.GetPointer());
				Assert::AreEqual(b, datum.GetPointer(1_z));

				datum.Set(c, 1_z);
				Assert::AreEqual(c, datum.GetPointer(1_z));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Set(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &a] { datum.Set(a, 3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetPointer(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.GetPointer(3_z); }, L"Expected an exception but none was thrown");

				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetPointer(); }, L"Expected an exception but none was thrown");
				datum.Clear();
				const Datum constEmptyDatum{ datum };
				Assert::ExpectException<exception>([&constEmptyDatum] { constEmptyDatum.GetPointer(); }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestSetFromStringAndToString)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);
				const int z(0);

				string sa = "10"s;
				string sb = "20"s;
				string sc = "30"s;

				Datum datum{ z, z, z };
				Assert::AreEqual(datum.GetInteger(), z);
				datum.SetFromString(sa);
				Assert::AreEqual(datum.GetInteger(), a);
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.GetInteger(1_z), z);
				datum.SetFromString(sb, 1_z);
				Assert::AreEqual(datum.GetInteger(1_z), b);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.GetInteger(2_z), z);
				datum.SetFromString(sc, 2_z);
				Assert::AreEqual(datum.GetInteger(2_z), c);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);
				const float z(0.f);

				string sa = to_string(a);
				string sb = to_string(b);
				string sc = to_string(c);

				Datum datum{ z, z, z };
				Assert::AreEqual(datum.GetFloat(), z);
				datum.SetFromString(sa);
				Assert::AreEqual(datum.GetFloat(), a);
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.GetFloat(1_z), z);
				datum.SetFromString(sb, 1_z);
				Assert::AreEqual(datum.GetFloat(1_z), b);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.GetFloat(2_z), z);
				datum.SetFromString(sc, 2_z);
				Assert::AreEqual(datum.GetFloat(2_z), c);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);
				const vec4 z(0.f);

				string sa = to_string(a);
				string sb = to_string(b);
				string sc = to_string(c);

				Datum datum{ z, z, z };
				Assert::AreEqual(datum.GetVector(), z);
				datum.SetFromString(sa);
				Assert::AreEqual(datum.GetVector(), a);
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.GetVector(1_z), z);
				datum.SetFromString(sb, 1_z);
				Assert::AreEqual(datum.GetVector(1_z), b);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.GetVector(2_z), z);
				datum.SetFromString(sc, 2_z);
				Assert::AreEqual(datum.GetVector(2_z), c);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);
				const mat4 z(0.f);

				string sa = to_string(a);
				string sb = to_string(b);
				string sc = to_string(c);

				Datum datum{ z, z, z };
				Assert::AreEqual(datum.GetMatrix(), z);
				datum.SetFromString(sa);
				Assert::AreEqual(datum.GetMatrix(), a);
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.GetMatrix(1_z), z);
				datum.SetFromString(sb, 1_z);
				Assert::AreEqual(datum.GetMatrix(1_z), b);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.GetMatrix(2_z), z);
				datum.SetFromString(sc, 2_z);
				Assert::AreEqual(datum.GetMatrix(2_z), c);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };
				const string z(""s);

				string sa = "Hello"s;
				string sb = "World"s;
				string sc = "!!!"s;

				Datum datum{ z, z, z };
				Assert::AreEqual(datum.GetString(), z);
				datum.SetFromString(sa);
				Assert::AreEqual(datum.GetString(), a);
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.GetString(1_z), z);
				datum.SetFromString(sb, 1_z);
				Assert::AreEqual(datum.GetString(1_z), b);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.GetString(2_z), z);
				datum.SetFromString(sc, 2_z);
				Assert::AreEqual(datum.GetString(2_z), c);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				string sa = a->ToString();
				string sb = b->ToString();
				string sc = c->ToString();

				Datum datum{ a, b, c };
				Assert::AreEqual(datum.ToString(), sa);
				Assert::AreEqual(datum.ToString(1_z), sb);
				Assert::AreEqual(datum.ToString(2_z), sc);
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				string sa = a->ToString();
				string sb = b->ToString();
				string sc = c->ToString();

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.SetFromString(""); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.ToString(); }, L"Expected an exception but none was thrown");

				datum.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<exception>([&datum, &sa] { datum.SetFromString(sa); }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestPushBackAndPopBackAndFrontAndBack)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontInteger(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackInteger(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::Integer);
				Assert::ExpectException<exception>([&datum] { datum.FrontInteger(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackInteger(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetInteger());
				Assert::AreEqual(a, datum.FrontInteger());
				Assert::AreEqual(a, datum.BackInteger());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetInteger(1_z));
				Assert::AreEqual(a, datum.FrontInteger());
				Assert::AreEqual(b, datum.BackInteger());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetInteger(2_z));
				Assert::AreEqual(a, datum.FrontInteger());
				Assert::AreEqual(c, datum.BackInteger());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetInteger(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetInteger(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(10); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontInteger(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackInteger(); }, L"Expected an exception but none was thrown");

				datum.PushBack(30);

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontInteger());
					Assert::AreEqual(c, constDatum.BackInteger());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontInteger(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackInteger(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ 10.f };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontInteger(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackInteger(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack(40);
				datum.PushBack(50);
				Assert::AreEqual(8_z, datum.Capacity());
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontFloat(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackFloat(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&datum] { datum.FrontFloat(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackFloat(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetFloat());
				Assert::AreEqual(a, datum.FrontFloat());
				Assert::AreEqual(a, datum.BackFloat());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetFloat(1_z));
				Assert::AreEqual(a, datum.FrontFloat());
				Assert::AreEqual(b, datum.BackFloat());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetFloat(2_z));
				Assert::AreEqual(a, datum.FrontFloat());
				Assert::AreEqual(c, datum.BackFloat());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetFloat(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetFloat(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(10.f); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontFloat(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackFloat(); }, L"Expected an exception but none was thrown");

				datum.PushBack(30.f);

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontFloat());
					Assert::AreEqual(c, constDatum.BackFloat());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontFloat(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackFloat(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ 10 };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontFloat(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackFloat(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack(40.f);
				datum.PushBack(50.f);
				Assert::AreEqual(8_z, datum.Capacity());
				
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontVector(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackVector(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::Vector);
				Assert::ExpectException<exception>([&datum] { datum.FrontVector(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackVector(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetVector());
				Assert::AreEqual(a, datum.FrontVector());
				Assert::AreEqual(a, datum.BackVector());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetVector(1_z));
				Assert::AreEqual(a, datum.FrontVector());
				Assert::AreEqual(b, datum.BackVector());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetVector(2_z));
				Assert::AreEqual(a, datum.FrontVector());
				Assert::AreEqual(c, datum.BackVector());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetVector(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetVector(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(vec4(10.f)); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontVector(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackVector(); }, L"Expected an exception but none was thrown");

				datum.PushBack(vec4(30.f));

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontVector());
					Assert::AreEqual(c, constDatum.BackVector());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontVector(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackVector(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ mat4(10.f) };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontVector(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackVector(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack(vec4(40.f));
				datum.PushBack(vec4(50.f));
				Assert::AreEqual(8_z, datum.Capacity());
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontMatrix(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackMatrix(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::Matrix);
				Assert::ExpectException<exception>([&datum] { datum.FrontMatrix(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackMatrix(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetMatrix());
				Assert::AreEqual(a, datum.FrontMatrix());
				Assert::AreEqual(a, datum.BackMatrix());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetMatrix(1_z));
				Assert::AreEqual(a, datum.FrontMatrix());
				Assert::AreEqual(b, datum.BackMatrix());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetMatrix(2_z));
				Assert::AreEqual(a, datum.FrontMatrix());
				Assert::AreEqual(c, datum.BackMatrix());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetMatrix(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetMatrix(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(mat4(10.f)); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontMatrix(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackMatrix(); }, L"Expected an exception but none was thrown");

				datum.PushBack(mat4(30.f));

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontMatrix());
					Assert::AreEqual(c, constDatum.BackMatrix());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontMatrix(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackMatrix(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ vec4(10.f) };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontMatrix(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackMatrix(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack(mat4(40.f));
				datum.PushBack(mat4(50.f));
				Assert::AreEqual(8_z, datum.Capacity());
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };
				
				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontString(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackString(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::String);
				Assert::ExpectException<exception>([&datum] { datum.FrontString(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackString(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetString());
				Assert::AreEqual(a, datum.FrontString());
				Assert::AreEqual(a, datum.BackString());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetString(1_z));
				Assert::AreEqual(a, datum.FrontString());
				Assert::AreEqual(b, datum.BackString());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetString(2_z));
				Assert::AreEqual(a, datum.FrontString());
				Assert::AreEqual(c, datum.BackString());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetString(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetString(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack("Hello"s); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontString(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackString(); }, L"Expected an exception but none was thrown");

				datum.PushBack("!!!"s);

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontString());
					Assert::AreEqual(c, constDatum.BackString());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontString(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackString(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ vec4(10.f) };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontString(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackString(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack("str"s);
				datum.PushBack("otherstr"s);
				Assert::AreEqual(8_z, datum.Capacity());
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				Datum datum{ };
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.FrontPointer(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackPointer(); }, L"Expected an exception but none was thrown");
				datum.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<exception>([&datum] { datum.FrontPointer(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.BackPointer(); }, L"Expected an exception but none was thrown");
				datum.PopBack();
				datum.PushBack(a);
				Assert::AreEqual(a, datum.GetPointer());
				Assert::AreEqual(a, datum.FrontPointer());
				Assert::AreEqual(a, datum.BackPointer());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.GetPointer(1_z));
				Assert::AreEqual(a, datum.FrontPointer());
				Assert::AreEqual(b, datum.BackPointer());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.GetPointer(2_z));
				Assert::AreEqual(a, datum.FrontPointer());
				Assert::AreEqual(c, datum.BackPointer());
				datum.PopBack();
				Assert::AreEqual(2_z, datum.Size());
				Assert::AreEqual(4_z, datum.Capacity());
				Assert::AreEqual(b, datum.GetPointer(1_z));
				Assert::ExpectException<exception>([&datum] { datum.GetPointer(2_z); }, L"Expected an exception but none was thrown");

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { Foo a(10); unknownDatum.PushBack(&a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.FrontPointer(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.BackPointer(); }, L"Expected an exception but none was thrown");

				datum.PushBack(&fc);

				{
					const Datum& constDatum{ datum };
					Assert::AreEqual(a, constDatum.FrontPointer());
					Assert::AreEqual(c, constDatum.BackPointer());
				}
				{
					Datum d{ a };
					d.Clear();
					const Datum& constDatum{ d };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontPointer(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackPointer(); }, L"Expected an exception but none was thrown");
				}
				{
					const Datum& constDatum{ "Hello"s };
					Assert::ExpectException<exception>([&constDatum] { constDatum.FrontPointer(); }, L"Expected an exception but none was thrown");
					Assert::ExpectException<exception>([&constDatum] { constDatum.BackPointer(); }, L"Expected an exception but none was thrown");
				}

				datum.PushBack(&fc);
				datum.PushBack(&fb);
				Assert::AreEqual(8_z, datum.Capacity());
			}
		}

		TEST_METHOD(TestRemoveAndIndexOf)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontInteger());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackInteger());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontInteger());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackInteger());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontFloat());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackFloat());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontFloat());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackFloat());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);
				
				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontVector());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackVector());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontVector());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackVector());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontMatrix());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackMatrix());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontMatrix());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackMatrix());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontString());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackString());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontString());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackString());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				Datum datum{ a, b, c };
				const Datum constDatum{ datum };
				Assert::AreEqual(a, datum.FrontPointer());
				Assert::AreEqual(0_z, datum.IndexOf(a));
				Assert::AreEqual(1_z, datum.IndexOf(b));
				Assert::AreEqual(2_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackPointer());

				Assert::AreEqual(0_z, constDatum.IndexOf(a));
				Assert::AreEqual(1_z, constDatum.IndexOf(b));
				Assert::AreEqual(2_z, constDatum.IndexOf(c));

				datum.Remove(a);
				Assert::AreEqual(b, datum.FrontPointer());
				Assert::AreEqual(0_z, datum.IndexOf(b));
				Assert::AreEqual(1_z, datum.IndexOf(c));
				Assert::AreEqual(c, datum.BackPointer());

				Assert::ExpectException<exception>([&datum, &a] { datum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::AreEqual(datum.Size(), datum.IndexOf(a));

				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.Remove(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&unknownDatum, &a] { unknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum, &a] { constUnknownDatum.IndexOf(a); }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestSetStorage)
		{
			{
				const int a(10);
				const int b(20);
				const int c(30);

				int data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::Integer);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PushBack(10); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum datumb{ datum };
				Assert::AreEqual(datum, datumb);

				Datum datumc;
				datumc = datum;
				Assert::AreEqual(datum, datumc);
			}
			{
				const float a(10.f);
				const float b(20.f);
				const float c(30.f);

				float data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::Float);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PushBack(10.f); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Integer);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");
			}
			{
				const vec4 a(10.f);
				const vec4 b(20.f);
				const vec4 c(30.f);

				vec4 data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::Vector);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PushBack(vec4(10.f)); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");
			}
			{
				const mat4 a(10.f);
				const mat4 b(20.f);
				const mat4 c(30.f);

				mat4 data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::Matrix);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PushBack(mat4(10.f)); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");
			}
			{
				const string a{ "Hello"s };
				const string b{ "World"s };
				const string c{ "!!!"s };

				string data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::String);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PushBack("Hello"s); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");
			}
			{
				Foo fa{ 10 };
				Foo fb{ 20 };
				Foo fc{ 30 };
				RTTI* a = &fa;
				RTTI* b = &fb;
				RTTI* c = &fc;

				RTTI* data[3] = { a, b, c };

				Datum datum{ };
				datum.SetType(Datum::DatumType::Pointer);
				datum.SetStorage(data, 3_z);
				Assert::ExpectException<exception>([&datum, &a] { datum.PushBack(a); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &fa] { datum.PushBack(&fa); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.PopBack(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Reserve(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Resize(10_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.Clear(); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum, &data] { datum.SetStorage(data, 0_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&datum] { datum.RemoveAt(0_z); }, L"Expected an exception but none was thrown");

				Datum otherDatum{ };
				otherDatum.SetType(Datum::DatumType::Float);
				Assert::ExpectException<exception>([&otherDatum, &data] { otherDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");

				Datum nonEmptyDatum{ a, b, c };
				Assert::ExpectException<exception>([&nonEmptyDatum, &data] { nonEmptyDatum.SetStorage(data, 3_z); }, L"Expected an exception but none was thrown");
			}
		}

		TEST_METHOD(TestScopeStuffAndExtraStuff)
		{
			{
				Scope a{ };
				Scope b{ };

				Datum datum{ };
				datum = a;
				Assert::AreEqual(1_z, datum.Size());
				Assert::AreEqual(1_z, datum.Capacity());

				const Datum& constDatum{ datum };
				Assert::AreEqual(a, constDatum[0]);

				Assert::ExpectException<exception>([&datum] { datum.GetScope(3_z); }, L"Expected an exception but none was thrown");
				Assert::ExpectException<exception>([&constDatum] { constDatum.GetScope(3_z); }, L"Expected an exception but none was thrown");
			
				Datum unknownDatum{ };
				Assert::ExpectException<exception>([&unknownDatum] { unknownDatum.GetScope(); }, L"Expected an exception but none was thrown");
				const Datum constUnknownDatum{ };
				Assert::ExpectException<exception>([&constUnknownDatum] { constUnknownDatum.GetScope(); }, L"Expected an exception but none was thrown");
			}
		}


	private:
		static _CrtMemState _startMemState; // or static inline and no extra declaration
	};

	_CrtMemState DatumTest::_startMemState;

}
