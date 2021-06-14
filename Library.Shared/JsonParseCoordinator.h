#pragma once

#include <gsl/gsl>

#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// A parser that translates from Json into a configuration for the game engine.
	/// </summary>
	class JsonParseCoordinator final
	{
	public:
		class SharedData : public FieaGameEngine::RTTI
		{
			friend JsonParseCoordinator;

			RTTI_DECLARATIONS(SharedData, RTTI);

		public:
			virtual ~SharedData() = default;

			/// <summary>
			/// Initializes this Shared Data, always called from derived Initializations.
			/// </summary>
			virtual void Initialize();
			/// <summary>
			/// Overridden implementations will create an instance of the shared data. This is a so-called “virtual constructor”.
			/// </summary>
			/// <returns>gls owner of shared data</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;
			/// <summary>
			/// Cleanup SharedData
			/// </summary>
			virtual inline void Cleanup() {};

			/// <summary>
			/// Returns the address of the JsonParseMaster associated with this object.
			/// </summary>
			/// <returns>Address of the JsonParseMaster associated with this object.</returns>
			JsonParseCoordinator* GetJsonParseCoordinator();
			/// <summary>
			/// Returns the address of the JsonParseMaster associated with this object.
			/// </summary>
			/// <returns>Address of the JsonParseMaster associated with this object.</returns>
			const JsonParseCoordinator* GetJsonParseCoordinator() const;

			/// <summary>
			/// Returns the current nesting depth.
			/// </summary>
			/// <returns>Current nesting depth.</returns>
			std::uint32_t Depth() const;

		private:
			void SetJsonParseCoordinator(JsonParseCoordinator* coordinator);

			void IncrementDepth();
			void DecrementDepth();

			JsonParseCoordinator* _coordinator{ nullptr };
			std::uint32_t _depth{ 0 };
		};

		explicit JsonParseCoordinator() = default;
		/// <summary>
		/// Given a reference to a SharedData object, initializes this object.
		/// </summary>
		/// <param name="sharedData">Reference to a SharedData object</param>
		explicit JsonParseCoordinator(SharedData& sharedData);
		JsonParseCoordinator(const JsonParseCoordinator& other) = default;
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = default;
		/// <summary>
		/// Move Constructor for a JsonParseCoordinator
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;
		/// <summary>
		/// Move Operator for a JsonParseCoordinator
		/// </summary>
		/// <param name="other">other coordinator to move</param>
		/// <returns>moved coordinator</returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;
		/// <summary>
		/// Note that the “original” parser does not “own” its helpers, in the sense that the caller
		/// created the helpers, but any cloned parse masters do own their helpers, since the master
		/// allocated the cloned helpers. Therefore this only delete helpers of cloned parsers.
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		/// Initializes this JsonParseCoordinator
		/// </summary>
		void Initialize();

		/// <summary>
		/// Duplicates this object and returns its address. This differs from a copy constructor
		/// in that technically a copy constructor should make a duplicate which behaves exactly
		/// as the original such that a copy constructor could be used to “move” an object in memory.
		/// We cannot do that with this object so instead of confusing the situation by abusing
		/// the copy constructor, we give it another name.
		/// </summary>
		/// <returns>new Scope</returns>
		virtual gsl::owner<JsonParseCoordinator*> Clone() const;
		/// <summary>
		/// Is this Coordinator a clone?
		/// </summary>
		/// <returns>Is a Clone</returns>
		bool IsClone() const;

		/// <summary>
		/// Vector of all helpers in this coordinator.
		/// </summary>
		/// <returns>Vector of helpers</returns>
		Vector<IJsonParseHelper*> Helpers();
		/// <summary>
		/// Given a reference to an IJsonParseHelper object, adds it to the vector.
		/// </summary>
		/// <param name="helper">reference to an IJsonParseHelper object</param>
		/// <exception cref="runtime_error">cannot add helper to a clone</exception>
		void AddHelper(IJsonParseHelper& helper);
		/// <summary>
		/// Given a reference to an IJsonParseHelper object, removes it from the list.
		/// </summary>
		/// <param name="helper">reference to an IJsonParseHelper object</param>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parses a string of Json data.
		/// </summary>
		/// <param name="data">string of json data</param>
		void Parse(const std::string& data);
		/// <summary>
		/// Given a filename, reads in the file and parses it.
		/// </summary>
		/// <param name="filename">filename to parse</param>
		/// <exception cref="invalid_argument">thrown if filename does not exist</exception>
		void ParseFromFile(const std::string& filename);
		/// <summary>
		/// Parses a string stream of Json data (std::stringstream).
		/// </summary>
		/// <param name="stream">string stream of Json data</param>
		void Parse(std::istream& stream);

		/// <summary>
		/// Return the const path for the file being parsed, passed into ParseFromFile.
		/// (Useful for reporting errors encountered while parsing.)
		/// </summary>
		/// <returns>const filename</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Returns the address of the SharedData associated with this object.
		/// </summary>
		/// <returns>address of the SharedData associated with this object.</returns>
		SharedData& GetSharedData();
		/// <summary>
		/// Returns the const address of the SharedData associated with this const object.
		/// </summary>
		/// <returns>const address of the SharedData associated with this object.</returns>
		const SharedData& GetSharedData() const;
		/// <summary>
		/// Given the address of a SharedData object, associates it with this object.
		/// </summary>
		/// <param name="sharedData">address of a SharedData object to associate with this object.</param>
		/// <exception cref="runtime_error">Cannot Set Shared Data to Clone</exception>
		void SetSharedData(SharedData& sharedData);

	private:
		void ParseMembers(const Json::Value& val);
		void Parse(const std::string& key, const Json::Value& val, bool isArray);

		void ParseHandlerHelper(const std::string& key, const Json::Value& val, bool isArray, size_t index = 0_z);

		void Clear();
		void Cleanup();

		SharedData* _sharedData{ nullptr };
		std::string _filename;
		bool _isClone{ false };
		Vector<IJsonParseHelper*> _helpers;

	};
}

