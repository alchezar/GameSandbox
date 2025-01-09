// Copyright © 2025, Ivan Kinder

#include "Settings/P14GameUserSettings.h"

#include "Settings/P14GameSetting.h"

#define LOCTEXT_NAMESPACE "P14GameUserSettings"

UP14GameUserSettings::UP14GameUserSettings()
{
	VideoSettingsOptions = {
		{LOCTEXT("FVXQualityLow_Loc", "Low"), 0},
		{LOCTEXT("FVXQualityMedium_Loc", "Medium"), 1},
		{LOCTEXT("FVXQualityHigh_Loc", "High"), 2},
		{LOCTEXT("FVXQualityEpic_Loc", "Epic"), 3}
	};

	// clang-format off
	CreateSetting(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"),
		[this]() -> int32 { return GetAntiAliasingQuality(); },
		[this](const int32 Value) -> void { SetAntiAliasingQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("Textures_Loc", "Textures"),
		[this]() -> int32 { return GetTextureQuality(); },
		[this](const int32 Value) -> void { SetTextureQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"),
		[this]() -> int32 { return GetGlobalIlluminationQuality(); },
		[this](const int32 Value) -> void { SetGlobalIlluminationQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("Shadows_Loc", "Shadows"),
		[this]() -> int32 { return GetShadingQuality(); },
		[this](const int32 Value) -> void { SetShadingQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("PostProcessing_Loc", "Post Processing"),
		[this]() -> int32 { return GetPostProcessingQuality(); },
		[this](const int32 Value) -> void { SetPostProcessingQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("Reflection_Loc", "Reflection"),
		[this]() -> int32 { return GetReflectionQuality(); },
		[this](const int32 Value) -> void { SetReflectionQuality(Value); ApplySettings(false); });
	CreateSetting(LOCTEXT("VisualEffect_Loc", "Visual Effect"),
		[this]() -> int32 { return GetVisualEffectQuality(); },
		[this](const int32 Value) -> void { SetVisualEffectQuality(Value); ApplySettings(false); });
	// clang-format on
}

UP14GameUserSettings* UP14GameUserSettings::Get()
{
	if (!GEngine)
	{
		return nullptr;
	}

	return Cast<UP14GameUserSettings>(GetGameUserSettings());
}

void UP14GameUserSettings::RunBenchmark()
{
	RunHardwareBenchmark();
	ApplySettings(false);
	OnSettingsUpdated.Broadcast();
}

void UP14GameUserSettings::CreateSetting(const FText& InName, TFunction<int32()>&& InGetter, TFunction<void(int32)>&& InSetter)
{
	UP14GameSetting* Setting = NewObject<UP14GameSetting>();
	check(Setting)

	Setting->Init(InName, VideoSettingsOptions, MoveTemp(InGetter), MoveTemp(InSetter));
	Settings.Add(Setting);
}

#undef LOCTEXT_NAMESPACE
