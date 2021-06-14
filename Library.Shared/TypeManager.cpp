#include "pch.h"

#include "TypeManager.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{
	Vector<Signature> TypeManager::GetSignaturesForType(RTTI::IdType typeId)
	{
		return _signatureMap.At(typeId);
	}

	const HashMap<RTTI::IdType, Vector<Signature>>& TypeManager::Types()
	{
		return _signatureMap;
	}

	void TypeManager::AddType(RTTI::IdType typeId, Vector<Signature> signatures)
	{
		if (_signatureMap.ContainsKey(typeId))
		{
			throw std::runtime_error("Type already registered");
		}

		_signatureMap.Insert(make_pair(typeId, std::move(signatures)));
	}

	void TypeManager::RemoveType(RTTI::IdType typeId)
	{
		_signatureMap.Remove(typeId);
	}

	bool TypeManager::ContainsType(RTTI::IdType typeId)
	{
		return _signatureMap.Find(typeId) != _signatureMap.end();
	}

	void TypeManager::Clear()
	{
		_signatureMap.Clear();
	}
}
