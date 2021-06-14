#include "pch.h"

#include <CppUnitTest.h>
#include <crtdbg.h>
#include <exception>
#include <string>
#include <string.h>

#include "Foo.h"
#include "DefaultHash.h"
#include "ToStringSpecialization.h"
#include "SizeLiteral.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	template <>
	struct DefaultHash<Foo>
	{
		inline size_t operator()(const Foo& key) const
		{
			return static_cast<size_t>(key.Data());
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
	{
	public:

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_CLASS_CLEANUP(ClassCleanUp)
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

		TEST_METHOD(TestIntegerHash)
		{
			int a = 10;
			int b = 20;
			int c = 10;
			DefaultHash<int> hashFunctor;

			Assert::AreEqual(hashFunctor(a), hashFunctor(a));
			Assert::AreNotEqual(hashFunctor(a), hashFunctor(b));
			Assert::AreNotEqual(hashFunctor(b), hashFunctor(c));
			Assert::AreEqual(hashFunctor(a), hashFunctor(c));
		}

		TEST_METHOD(TestStringHash)
		{
			string a = "Hello"s;
			string b = "World"s;
			string c(a);
			DefaultHash<string> hashFunctor;

			Assert::AreEqual(hashFunctor(a), hashFunctor(a));
			Assert::AreNotEqual(hashFunctor(a), hashFunctor(b));
			Assert::AreNotEqual(hashFunctor(b), hashFunctor(c));
			Assert::AreEqual(hashFunctor(a), hashFunctor(c));
		}

		TEST_METHOD(TestWideStringHash)
		{
			wstring a = L"Hello"s;
			wstring b = L"World"s;
			wstring c(a);
			DefaultHash<wstring> hashFunctor;

			Assert::AreEqual(hashFunctor(a), hashFunctor(a));
			Assert::AreNotEqual(hashFunctor(a), hashFunctor(b));
			Assert::AreNotEqual(hashFunctor(b), hashFunctor(c));
			Assert::AreEqual(hashFunctor(a), hashFunctor(c));
		}

		TEST_METHOD(TestCStringHash)
		{
			const char* a = "Hello";
			const char* b = "World";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<const char*> hashFunctor;

			Assert::AreEqual(hashFunctor(a), hashFunctor(a));
			Assert::AreNotEqual(hashFunctor(a), hashFunctor(b));
			Assert::AreNotEqual(hashFunctor(b), hashFunctor(c));
			Assert::AreEqual(hashFunctor(a), hashFunctor(c));
		}

		TEST_METHOD(TestFooHash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(10);
			DefaultHash<Foo> hashFunctor;

			Assert::AreEqual(hashFunctor(a), hashFunctor(a));
			Assert::AreNotEqual(hashFunctor(a), hashFunctor(b));
			Assert::AreNotEqual(hashFunctor(b), hashFunctor(c));
			Assert::AreEqual(hashFunctor(a), hashFunctor(c));
		}

	private:
		static _CrtMemState _startMemState; // or static inline and no extra declaration
	};

	_CrtMemState DefaultHashTest::_startMemState;

}
