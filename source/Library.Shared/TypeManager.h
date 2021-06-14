#pragma once

#include "Attributed.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Signature Struct for FieaGameEngine
	/// </summary>
	struct Signature final
	{
		std::string Name;
		Datum::DatumType Type;
		size_t Size;
		size_t Offset;
	};

	/// <summary>
	/// FieaGameEngine's Type Manager, manages all types in the engine at runtime
	/// </summary>
	class TypeManager final
	{
	public:
		TypeManager() = delete;
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager operator=(const TypeManager&) = delete;
		TypeManager operator=(TypeManager&&) = delete;
		~TypeManager() = default;

		/// <summary>
		/// Gets all the signatures of the vector passed in.
		/// </summary>
		/// <param name="typeId">typeId to get signatures of</param>
		/// <returns>Vector of Signatures of type given</returns>
		static Vector<Signature> GetSignaturesForType(RTTI::IdType typeId);

		/// <summary>
		/// Returns map of all types in the manager
		/// </summary>
		/// <returns>map of types</returns>
		static const HashMap<RTTI::IdType, Vector<Signature>>& Types();

		/// <summary>
		/// Adds the type to the manager
		/// </summary>
		/// <param name="typeId">type id to add to the manager</param>
		/// <param name="signatures">signatures to add to the typeId</param>
		static void AddType(RTTI::IdType typeId, Vector<Signature> signatures);

		/// <summary>
		/// Removes type in the manager
		/// </summary>
		/// <param name="typeId">typeId to remove from manager</param>
		static void RemoveType(RTTI::IdType typeId);

		/// <summary>
		/// Does the Manager contain the given string name?
		/// </summary>
		/// <param name="typeId">type id to find in manager</param>
		/// <returns>true if manager contains the path</returns>
		static bool ContainsType(RTTI::IdType typeId);

		/// <summary>
		/// Clears the Manager of all it's types
		/// </summary>
		static void Clear();

	private:
		inline static HashMap<RTTI::IdType, Vector<Signature>> _signatureMap;
	};
}

