// Copyright © 2024, Ivan Kinder

#pragma once
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "UI/P14SettingOption.h"
#include "UI/P14VideoSettings.h"
#include "Utils/P14Utils.h"

namespace P14::Test
{
	/// Simulate the click on the next setting button.
	/// @param WorldContextObject The world context object.
	/// @param SettingIndex The index of the setting.
	inline void DoNextSettingClick(UObject* WorldContextObject, const int32 SettingIndex)
	{
		const auto* VideoSettings = FindWidgetByClass<UP14VideoSettings>(WorldContextObject);
		const auto* VerticalBox   = FindWidgetChildByName<UVerticalBox>(VideoSettings, "VideoSettingsContainer");
		const auto* SettingOption = Cast<UP14SettingOption>(VerticalBox->GetChildAt(SettingIndex));
		const auto* NextButton    = FindWidgetChildByName<UButton>(SettingOption, "NextSettingButton");
		NextButton->OnReleased.Broadcast();
	}

	/// Simulate the click on the benchmark button.
	/// @param WorldContextObject The world context object.
	inline void DoBenchmarkSettingClick(UObject* WorldContextObject)
	{
		const auto* VideoSettings   = FindWidgetByClass<UP14VideoSettings>(WorldContextObject);
		const auto* BenchmarkButton = FindWidgetChildByName<UButton>(VideoSettings, "RunBenchmarkButton");
		BenchmarkButton->OnReleased.Broadcast();
	}
}
