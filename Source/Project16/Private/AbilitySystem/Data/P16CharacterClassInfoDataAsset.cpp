// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16CharacterClassInfoDataAsset.h"

FP16CharacterClassDefaultInfo UP16CharacterClassInfoDataAsset::GetClassDefaultInfo(EP16CharacterClass CharacterClass)
{
	return CharacterClassInfoMap[CharacterClass];
}
