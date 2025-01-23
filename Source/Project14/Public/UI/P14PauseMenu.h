// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P14PauseMenu.generated.h"

class UButton;

UCLASS()
class PROJECT14_API UP14PauseMenu : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnCloseButtonCallback();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	UButton* CloseButton = nullptr;
};
