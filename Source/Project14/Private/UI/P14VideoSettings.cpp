// Copyright © 2024, Ivan Kinder

#include "UI/P14VideoSettings.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/P14GameUserSettings.h"
#include "UI/P14SettingOption.h"

DEFINE_LOG_CATEGORY_STATIC(LogP14VideoSettingsWidget, All, All)

void UP14VideoSettings::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UP14GameUserSettings* UserSettings = UP14GameUserSettings::Get();
	if (!UserSettings)
	{
		UE_LOG(LogP14VideoSettingsWidget, Error, TEXT("UP14GameUserSettings is nullptr"));
		return;
	}

	UserSettings->LoadSettings();
	const TArray<UP14GameSetting*>& Settings = UserSettings->GetVideoSettings();

	check(VideoSettingsContainer)
	VideoSettingsContainer->ClearChildren();

	for (UP14GameSetting* Setting : Settings)
	{
		UP14SettingOption* SettingWidget = CreateWidget<UP14SettingOption>(this, SettingOptionClass);
		check(SettingWidget)
		SettingWidget->Init(Setting);
		VideoSettingsContainer->AddChild(SettingWidget);
	}

	RunBenchmarkButton->OnReleased.AddDynamic(this, &ThisClass::OnRunBenchmarkButtonCallback);
	UserSettings->OnSettingsUpdated.AddUObject(this, &ThisClass::OnSettingsUpdatedCallback);
}

void UP14VideoSettings::OnRunBenchmarkButtonCallback()
{
	if (UP14GameUserSettings* UserSettings = UP14GameUserSettings::Get())
	{
		UserSettings->RunBenchmark();
	}
}

void UP14VideoSettings::OnSettingsUpdatedCallback()
{
	for (UWidget* Widget : VideoSettingsContainer->GetAllChildren())
	{
		if (UP14SettingOption* Setting = Cast<UP14SettingOption>(Widget))
		{
			Setting->UpdateTexts();
		}
	}
}
