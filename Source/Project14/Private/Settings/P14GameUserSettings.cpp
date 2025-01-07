// Copyright © 2025, Ivan Kinder

#include "Settings/P14GameUserSettings.h"

#include "Settings/P14GameSetting.h"

UP14GameUserSettings::UP14GameUserSettings()
{
	VfxOptions = {
		{"Low", 0},
		{"Medium", 1},
		{"High", 2},
		{"Epic", 3}
	};

	CreateSetting("Anti-Aliasing");
	CreateSetting("Textures");
	CreateSetting("Global Illumination");
	CreateSetting("Shadows");
	CreateSetting("Post Processing");
}

UP14GameUserSettings* UP14GameUserSettings::Get()
{
	if (!GEngine)
	{
		return nullptr;
	}

	return Cast<UP14GameUserSettings>(GetGameUserSettings());
}

void UP14GameUserSettings::CreateSetting(const FString& InName)
{
	UP14GameSetting* Setting = NewObject<UP14GameSetting>();
	check(Setting)

	Setting->SetName(InName);
	Setting->SetOptions(VfxOptions);
	Settings.Add(Setting);
}
