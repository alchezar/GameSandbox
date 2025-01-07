// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
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

private:
	void CreateSetting(const FString& InName);

	/* ------------------------------ Fields ------------------------------- */
private:
	UPROPERTY()
	TArray<UP14GameSetting*>   Settings   = {};
	TArray<FP14SettingsOption> VfxOptions = {};
};
