#include "pch.h"

#include <fstream>

#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"

using namespace std;
using namespace FieaGameEngine;

namespace FieaGameEngine
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonParseCoordinator::SharedData);

	void JsonParseCoordinator::SharedData::Initialize()
	{
		_depth = 0;
	}

	JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator()
	{
		return _coordinator;
	}

	const JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator() const
	{
		return _coordinator;
	}

	void JsonParseCoordinator::SharedData::IncrementDepth()
	{
		++_depth;
	}

	void JsonParseCoordinator::SharedData::DecrementDepth()
	{
		if (_depth > 0) --_depth;
	}

	std::uint32_t JsonParseCoordinator::SharedData::Depth() const
	{
		return _depth;
	}

	void JsonParseCoordinator::SharedData::SetJsonParseCoordinator(JsonParseCoordinator* coordinator)
	{
		_coordinator = coordinator;
	}

#pragma endregion

#pragma region JsonParseCoordinator

	JsonParseCoordinator::JsonParseCoordinator(SharedData& sharedData) :
		_sharedData{ &sharedData }
	{
		_sharedData->SetJsonParseCoordinator(this);
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_sharedData{ other._sharedData }, _helpers{ std::move(other._helpers) }, _filename{ std::move(other._filename) }, _isClone{ other._isClone }
	{
		other._sharedData = nullptr;
		other._isClone = false;

		_sharedData->SetJsonParseCoordinator(this);
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		if (this != &other)
		{
			_sharedData = std::move(other._sharedData);
			_helpers = std::move(other._helpers);
			_filename = std::move(other._filename);
			_isClone = other._isClone;

			_sharedData->SetJsonParseCoordinator(this);
		}

		return *this;
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		Clear();
	}

	void JsonParseCoordinator::Initialize()
	{
		_sharedData->Initialize();

		for (auto helper : _helpers)
		{
			helper->Initialize();
		}

		_filename.clear();
	}

	gsl::owner<JsonParseCoordinator*> JsonParseCoordinator::Clone() const
	{
		JsonParseCoordinator* clone = new JsonParseCoordinator(*_sharedData->Create());
		clone->_isClone = true;
		clone->_filename = _filename;

		clone->_helpers.Reserve(_helpers.Size());
		for (auto helper : _helpers)
		{
			IJsonParseHelper* helperClone = helper->Create();
			clone->_helpers.PushBack(helperClone);
		}

		return clone;
	}

	bool JsonParseCoordinator::IsClone() const
	{
		return _isClone;
	}

	Vector<IJsonParseHelper*> JsonParseCoordinator::Helpers()
	{
		return _helpers;
	}

	void JsonParseCoordinator::AddHelper(IJsonParseHelper& helper)
	{
		if (_isClone)
		{
			throw std::runtime_error("Cannot Add Helper to Clone");
		}

		auto it = std::find_if(_helpers.begin(), _helpers.end(), [&helper](const IJsonParseHelper* h)
		{
			return helper.TypeIdInstance() == h->TypeIdInstance();
		});

		if (it != _helpers.end())
		{
			throw std::runtime_error("This helper, or one of the same type, has already been added");
		}

		_helpers.PushBack(&helper);
	}

	void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
	{
		_helpers.Remove(&helper);
		if (_isClone)
		{
			delete &helper;
		}
	}

	void JsonParseCoordinator::Parse(const std::string& data)
	{
		istringstream inputStream(data);
		Parse(inputStream);
	}

	void JsonParseCoordinator::ParseFromFile(const std::string& filename)
	{
		ifstream file(filename);
		if (!file.good())
		{
			throw std::invalid_argument("File does not exists");
		}

		Parse(file);
		_filename = filename;
	}

	void JsonParseCoordinator::Parse(std::istream& stream)
	{
		Initialize();
		Json::Value root;
		stream >> root;

		ParseMembers(root);
		Cleanup();
	}

	const std::string& JsonParseCoordinator::GetFileName() const
	{
		return _filename;
	}

	JsonParseCoordinator::SharedData& JsonParseCoordinator::GetSharedData()
	{
		return *_sharedData;
	}

	const JsonParseCoordinator::SharedData& JsonParseCoordinator::GetSharedData() const
	{
		return *_sharedData;
	}

	void JsonParseCoordinator::SetSharedData(JsonParseCoordinator::SharedData& sharedData)
	{
		if (_isClone)
		{
			throw std::runtime_error("Cannot Set Shared Data to Clone");
		}
		_sharedData = &sharedData;
		_sharedData->SetJsonParseCoordinator(this);
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& val)
	{
		if (val.size() > 0)
		{
			const vector<string> keys = val.getMemberNames();
			_sharedData->IncrementDepth();
			for (const string& key : keys)
			{
				bool isArray = val[key].isArray();
				Parse(key, val[key], isArray);
			}
			_sharedData->DecrementDepth();
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& val, bool isArray)
	{
		if (isArray)
		{
			for (Json::ArrayIndex i = 0; i < val.size(); ++i)
			{
				ParseHandlerHelper(key, val[i], isArray, i);
			}
		}
		else
		{
			ParseHandlerHelper(key, val, isArray);
		}
	}

	void JsonParseCoordinator::ParseHandlerHelper(const std::string& key, const Json::Value& val, bool isArray, size_t index)
	{
		for (auto helper : _helpers)
		{
			if (helper->StartHandler(*_sharedData, key, val, isArray, index))
			{
				if (val.isObject()) ParseMembers(val);
				helper->EndHandler(*_sharedData, key, isArray);
				break;
			}
		}
	}

	void JsonParseCoordinator::Clear()
	{
		if (_isClone)
		{
			for (auto helper : _helpers)
			{
				RemoveHelper(*helper);
			}
			delete _sharedData;
		}
	}

	void JsonParseCoordinator::Cleanup()
	{
		_sharedData->Cleanup();

		for (IJsonParseHelper* helper : _helpers)
		{
			helper->Cleanup();
		}
	}

#pragma endregion
}
