// Copyright © 2024, Ivan Kinder

#include "UI/P14SettingOption.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UP14SettingOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(SettingDisplayName)
	check(SettingCurrentValue)

	PrevSettingButton->OnReleased.AddDynamic(this, &ThisClass::OnPrevSettingButtonCallback);
	NextSettingButton->OnReleased.AddDynamic(this, &ThisClass::OnNextSettingButtonCallback);
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

	SettingDisplayName->SetText(Setting->GetOptionName());
	SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
}

void UP14SettingOption::OnPrevSettingButtonCallback()
{
	Setting->UpdateCurrentValue(-1);
	UpdateTexts();
}

void UP14SettingOption::OnNextSettingButtonCallback()
{
	Setting->UpdateCurrentValue(1);
	UpdateTexts();
}
