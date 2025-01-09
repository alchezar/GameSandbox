// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P14GameSetting.generated.h"

struct FP14SettingsOption
{
	FText Name;
	int32 Value;
};

UCLASS()
class UP14GameSetting : public UObject
{
	GENERATED_BODY()

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD FText              GetOptionName() const;
	_NODISCARD FP14SettingsOption GetCurrentOption() const;

	void  Init(const FText& InName, const TArray<FP14SettingsOption>& InOptions, TFunction<int32()>&& InGetter, TFunction<void(int32)>&& InSetter);
	int32 GetCurrentValue() const;
	void  UpdateCurrentValue(int32 Offset) const;

protected:
	void SetName(const FText& InName);
	void SetOptions(const TArray<FP14SettingsOption>& InOptions);
	void SetGetter(TFunction<int32()>&& InFunc);
	void SetSetter(TFunction<void(int32)>&& InFunc);

	/* ------------------------------ Fields ------------------------------- */
private:
	FText                      Name;
	TArray<FP14SettingsOption> Options;
	TFunction<int32()>         Getter;
	TFunction<void(int32)>     Setter;
};
