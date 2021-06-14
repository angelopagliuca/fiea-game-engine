#pragma once

#include <json/json.h>

#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Interface for helpers to parse JSON using the JsonParseCoodinator Parser.
	/// </summary>
	class IJsonParseHelper : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initializes this helper. This will get called just before each file is parsed.
		/// </summary>
		virtual inline void Initialize() {};
		/// <summary>
		/// Cleanup this helper
		/// </summary>
		virtual inline void Cleanup() {};
		/// <summary>
		/// Given a shared data reference, a string for the Json key, a reference to the Json::Value object,
		/// and a bool indicating if the value is an array element, attempt to handle the key/value pair.
		/// If this routine does indeed handle the pair, return true, otherwise return false.
		/// </summary>
		/// <param name="sharedData">shared data reference</param>
		/// <param name="key">string for the Json key</param>
		/// <param name="value">reference to the Json::Value object</param>
		/// <param name="isArray">bool indicating if the value is an array element</param>
		/// <returns>If this routine does indeed handle the pair, return true, otherwise return false.</returns>
		virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index) = 0;
		/// <summary>
		/// Given a shared data reference, a string for the Json key, attempt to complete the handling of the element pair.
		/// If this routine does indeed handle the pair, return true, otherwise return false.
		/// </summary>
		/// <param name="sharedData">shared data reference</param>
		/// <param name="key">string for the Json key</param>
		/// <returns>If this routine does indeed handle the pair, return true, otherwise return false.</returns>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) = 0;
		/// <summary>
		/// Overridden implementations will create an instance of the helper. This is a so-called “virtual constructor”.
		/// </summary>
		/// <returns></returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
	};
}
