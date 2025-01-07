// Copyright © 2025, Ivan Kinder

#include "Settings/P14GameSetting.h"

FString UP14GameSetting::GetOptionName() const
{
	return Name;
}

FP14SettingsOption UP14GameSetting::GetCurrentOption() const
{
	return Options[0];
}

void UP14GameSetting::SetName(const FString& InName)
{
	Name = InName;
}

void UP14GameSetting::SetOptions(const TArray<FP14SettingsOption>& InOptions)
{
	Options = InOptions;
}
