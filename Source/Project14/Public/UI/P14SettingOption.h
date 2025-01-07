// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Settings/P14GameSetting.h"
#include "P14SettingOption.generated.h"

class UTextBlock;

UCLASS()
class PROJECT14_API UP14SettingOption : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
public:
	void Init(UP14GameSetting* InSetting);

protected:
	void UpdateTexts();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	UTextBlock* SettingDisplayName;
	UPROPERTY(Transient, meta = (BindWidget))
	UTextBlock* SettingCurrentValue;

private:
	TWeakObjectPtr<UP14GameSetting> Setting;
};
