// Copyright (C) 2023, IKinder

#include "STU_GameInstance.h"

FLevelData USTU_GameInstance::GetStartupLevel() const
{
	return StartupLevel;
}

FName USTU_GameInstance::GetMainLevelName() const
{
	return MainLevelName;
}

TArray<FLevelData> USTU_GameInstance::GetLevelsData() const
{
	return LevelsData;
}

void USTU_GameInstance::SetStartupLevel(const FLevelData& Data)
{
	StartupLevel = Data;
}
