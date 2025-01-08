// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P14VideoSettings.generated.h"

class UButton;
class UP14SettingOption;
class UVerticalBox;

UCLASS()
class PROJECT14_API UP14VideoSettings : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnRunBenchmarkButtonCallback();
	void OnSettingsUpdatedCallback();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	UVerticalBox* VideoSettingsContainer;
	UPROPERTY(Transient, meta = (BindWidget))
	UButton* RunBenchmarkButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++|UI")
	TSubclassOf<UP14SettingOption> SettingOptionClass = nullptr;
};
