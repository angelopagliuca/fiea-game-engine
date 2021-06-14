#pragma once

#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Json Parse Helper that parses Json Table Files
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)
	public:

		struct StackFrame
		{
			StackFrame(const std::string& key, Datum::DatumType type, Scope* context) : Key(key), Type(type), Context(context) { };
			StackFrame(const std::string& key, Scope* context) : Key(key), Context(context) { };

			const std::string& Key;
			Datum::DatumType Type = Datum::DatumType::Unknown;
			std::string ClassName;
			Scope* Context = nullptr;
		};

		class SharedData final : public JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseCoordinator::SharedData);

		public:
			SharedData() = default;
			explicit SharedData(Scope& scope);

			/// <summary>
			/// Initializes this Shared Data, always called from derived Initializations.
			/// </summary>
			virtual void Initialize() override;
			/// <summary>
			/// Virtual Constructor of SharedData.
			/// </summary>
			/// <returns>gls owner of shared data</returns>
			virtual gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// Sets the Context Scope of the SharedData
			/// </summary>
			/// <param name="scope">scope to set as context</param>
			void SetScope(Scope* scope);

			/// <summary>
			/// Get's SharedData Scope Context
			/// </summary>
			/// <returns>Context</returns>
			Scope* GetScope();
			/// <summary>
			/// Get's SharedData Scope Context
			/// </summary>
			/// <returns>Context</returns>
			const Scope* GetScope() const;

		private:
			Scope* Context{ nullptr };
		};

		/// <summary>
		/// Virtual Constructor of the Table Parse Helper
		/// </summary>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		/// <summary>
		/// Initializes Table Parse Helper
		/// </summary>
		virtual void Initialize() override;
		/// <summary>
		/// Helper determines if and how to "handle" the start of a name/value pair.
		/// </summary>
		virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index) override;
		/// <summary>
		/// Helper determines if and how to "handle" the end of a name/value pair.
		/// </summary>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) override;

	private:
		inline static const std::string TypeKey = "type";
		inline static const std::string ClassKey = "class";
		inline static const std::string ValueKey = "value";

		Stack<StackFrame> _contextStack;
	};
}

