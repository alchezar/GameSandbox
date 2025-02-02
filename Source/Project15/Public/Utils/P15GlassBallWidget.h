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
public:
	_NODISCARD TObjectPtr<UImage>                   GetGlassBallImage();
	_NODISCARD TObjectPtr<UMaterialInstanceDynamic> GetBallDynamicMaterial();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GlassBallImage;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BallDynamicMaterial = nullptr;
};
