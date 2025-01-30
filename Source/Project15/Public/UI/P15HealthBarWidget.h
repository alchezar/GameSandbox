// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P15HealthBarWidget.generated.h"

class AP15Character;
class UProgressBar;

UCLASS()
class PROJECT15_API UP15HealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION(BlueprintCallable)
	void FindOwner(AP15Character* CurrentOwner);

private:
	void OnHealthChangedCallback(const float NewHealthPercentage);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar = nullptr;
};
