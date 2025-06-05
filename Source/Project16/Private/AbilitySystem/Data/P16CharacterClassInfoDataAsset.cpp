// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16CharacterClassInfoDataAsset.h"

FP16CharacterClassDefaultInfo UP16CharacterClassInfoDataAsset::GetClassDefaultInfo(const EP16CharacterClass CharacterClass)
{
	FP16CharacterClassDefaultInfo Result = {};

	if (CharacterClassInfoMap.Contains(CharacterClass))
	{
		Result = CharacterClassInfoMap[CharacterClass];
	}

	return Result;
}
