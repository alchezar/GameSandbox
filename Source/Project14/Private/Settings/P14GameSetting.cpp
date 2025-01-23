// Copyright © 2025, Ivan Kinder

#include "Settings/P14GameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogP14GameSetting, All, All)

FText UP14GameSetting::GetOptionName() const
{
	return Name;
}

FP14SettingsOption UP14GameSetting::GetCurrentOption() const
{
	const FP14SettingsOption* FoundOption = Options.FindByPredicate([this](const FP14SettingsOption& EachOption) -> bool
	{
		return EachOption.Value == GetCurrentValue();
	});

	if (!FoundOption)
	{
		UE_LOG(LogP14GameSetting, Warning, TEXT("Option %s doesn't exist"), *Name.ToString());
		return FP14SettingsOption{};
	}

	return *FoundOption;
}

void UP14GameSetting::Init(const FText& InName, const TArray<FP14SettingsOption>& InOptions, TFunction<int32()>&& InGetter, TFunction<void(int32)>&& InSetter)
{
	Name    = InName;
	Options = InOptions;
	Getter  = InGetter;
	Setter  = InSetter;
}

int32 UP14GameSetting::GetCurrentValue() const
{
	if (!Getter)
	{
		UE_LOG(LogP14GameSetting, Warning, TEXT("Setting %s has no getter"), *Name.ToString());
		return INDEX_NONE;
	}

	return Getter();
}

void UP14GameSetting::UpdateCurrentValue(const int32 Offset) const
{
	int CorrectValue = (GetCurrentValue() + Offset) % Options.Num();
	if (CorrectValue < 0)
	{
		CorrectValue += Options.Num();
	}

	Setter(CorrectValue);
}

void UP14GameSetting::SetName(const FText& InName)
{
	Name = InName;
}

void UP14GameSetting::SetOptions(const TArray<FP14SettingsOption>& InOptions)
{
	Options = InOptions;
}

void UP14GameSetting::SetGetter(TFunction<int32()>&& InFunc)
{
	Getter = InFunc;
}

void UP14GameSetting::SetSetter(TFunction<void(int32)>&& InFunc)
{
	Setter = InFunc;
}
