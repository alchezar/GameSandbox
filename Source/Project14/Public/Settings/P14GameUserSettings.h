// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Utils/P14Types.h"
#include "P14GameUserSettings.generated.h"

struct FP14SettingsOption;
class UP14GameSetting;

UCLASS()
class PROJECT14_API UP14GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP14GameUserSettings();

	/* ------------------------------- This -------------------------------- */
public:
	static UP14GameUserSettings*               Get();
	_NODISCARD const TArray<UP14GameSetting*>& GetVideoSettings() const { return Settings; }
	void                                       RunBenchmark();

private:
	void CreateSetting(const FText& InName, TFunction<int32()>&& InGetter, TFunction<void(int32)>&& InSetter);

	/* ------------------------------ Fields ------------------------------- */
public:
	FP14OnSettingsUpdatedSignature OnSettingsUpdated;

private:
	UPROPERTY()
	TArray<UP14GameSetting*>   Settings             = {};
	TArray<FP14SettingsOption> VideoSettingsOptions = {};
};
