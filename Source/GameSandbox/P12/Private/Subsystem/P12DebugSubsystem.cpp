// Copyright (C) 2023, IKinder

#include "P12/Public/Subsystem/P12DebugSubsystem.h"

bool UP12DebugSubsystem::GetIsDebugCategoryEnabled(const FName& CategoryName) const
{
	const bool* bEnabled = DebugCategories.Find(CategoryName);
	return bEnabled && *bEnabled;
}

void UP12DebugSubsystem::EnableDebugCategory(const FName& CategoryName, bool bEnabled)
{
	DebugCategories.FindOrAdd(CategoryName);
	DebugCategories[CategoryName] = bEnabled;
}
