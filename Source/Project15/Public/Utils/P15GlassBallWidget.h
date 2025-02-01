// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P15GlassBallWidget.generated.h"

class UImage;

UCLASS()
class PROJECT15_API UP15GlassBallWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnHealthChangedCallback(const float NewHealthPercentage = 1.f);

	/* ------------------------------- This -------------------------------- */
protected:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GlassBallImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName HealthParameterName = "Health";

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicHealthMaterial = nullptr;
};
