// Copyright © 2024, Ivan Kinder

#include "UI/P14SettingOption.h"

#include "Components/TextBlock.h"

void UP14SettingOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(SettingDisplayName)
	check(SettingCurrentValue)
}

void UP14SettingOption::Init(UP14GameSetting* InSetting)
{
	Setting = MakeWeakObjectPtr(InSetting);
	check(Setting.IsValid())

	UpdateTexts();
}

void UP14SettingOption::UpdateTexts()
{
	if (!Setting.IsValid())
	{
		return;
	}

	SettingDisplayName->SetText(FText::FromString(Setting->GetOptionName()));
	SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
}
