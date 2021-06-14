#include "pch.h"

#include "JsonTableParseHelper.h"
#include "Factory.h"
#include <json/json.h>

using namespace std;

namespace FieaGameEngine
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData);

	JsonTableParseHelper::SharedData::SharedData(Scope& scope) :
		Context{ &scope }
	{
	}

	void JsonTableParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		return new SharedData();
	}

	void JsonTableParseHelper::SharedData::SetScope(Scope* scope)
	{
		Context = scope;
	}

	Scope* JsonTableParseHelper::SharedData::GetScope()
	{
		return Context;
	}

	const Scope* JsonTableParseHelper::SharedData::GetScope() const
	{
		return Context;
	}

#pragma endregion

#pragma region JsonTableParseHelper

	RTTI_DEFINITIONS(JsonTableParseHelper);

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
	}

	bool JsonTableParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index)
	{
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		if (key == TypeKey)
		{
			assert(_contextStack.Size() > 0_z);
			if (!value.isString()) throw std::runtime_error("Type must be a string");
			StackFrame& stackFrame = _contextStack.Top();
			Datum* datum = stackFrame.Context->Search(stackFrame.Key);
			assert(datum != nullptr);
			datum->SetType(Datum::DatumTypeMap.At(value.asString()));
			stackFrame.Type = Datum::DatumTypeMap.At(value.asString());
		}
		else if (key == ClassKey)
		{
			assert(_contextStack.Size() > 0_z);
			if (!value.isString()) throw std::runtime_error("Class must be a string");
			StackFrame& stackFrame = _contextStack.Top();
			stackFrame.ClassName = value.asString();
		}
		else if (key == ValueKey)
		{
			assert(_contextStack.Size() > 0_z);
			StackFrame& stackFrame = _contextStack.Top();

			if (stackFrame.Type == Datum::DatumType::Table)
			{
				if (!isArray) {
					const string& className = stackFrame.ClassName.empty() ? "Scope" : stackFrame.ClassName;
					Scope* nestedScope = Factory<Scope>::Create(className);
					assert(nestedScope != nullptr);
					stackFrame.Context->Adopt(*nestedScope, stackFrame.Key);
					_contextStack.Push({ key, Datum::DatumType::Table, nestedScope });
				}
			}
			else
			{
				Datum& datum = stackFrame.Context->Append(stackFrame.Key);
				
				switch (stackFrame.Type)
				{
					case Datum::DatumType::Integer:
						if (datum.IsExternal()) datum.Set(value.asInt(), index);
						else datum.PushBack(value.asInt());
						break;

					case Datum::DatumType::Float:
						if (datum.IsExternal()) datum.Set(value.asFloat(), index);
						else datum.PushBack(value.asFloat());
						break;

					default:
						if (datum.IsExternal()) datum.SetFromString(value.asString(), index);
						else datum.PushBackFromString(value.asString());
						break;
				}
			}
		}
		else {
			Scope* scope = { _contextStack.IsEmpty() ? customSharedData->GetScope() : _contextStack.Top().Context };
			assert(scope != nullptr);
			scope->Append(key);
			_contextStack.Push({ key, scope });
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray)
	{
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		const StackFrame& stackFrame = _contextStack.Top();
		if (&key == &stackFrame.Key && !isArray)
		{
			_contextStack.Pop();
		}

		return true;
	}
}