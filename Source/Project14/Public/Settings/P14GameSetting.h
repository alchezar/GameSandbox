// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P14GameSetting.generated.h"

USTRUCT()
struct FP14SettingsOption
{
	GENERATED_BODY()

	FString Name;
	int32   Value;
};

UCLASS()
class UP14GameSetting : public UObject
{
	GENERATED_BODY()

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD FString           GetOptionName() const;
	_NODISCARD FP14SettingsOption GetCurrentOption() const;

	void SetName(const FString& InName);
	void SetOptions(const TArray<FP14SettingsOption>& InOptions);

	/* ------------------------------ Fields ------------------------------- */
private:
	FString                   Name;
	TArray<FP14SettingsOption> Options;
};
