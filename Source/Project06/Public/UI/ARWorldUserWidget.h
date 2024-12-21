// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARWorldUserWidget.generated.h"

class UAREffectContainer;
class UImage;
class USizeBox;

UCLASS()
class PROJECT06_API UARWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNewHealthPercent(const float HealthPercent);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY()
	AActor* AttachedActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BindWidget))
	USizeBox* ParentSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BindWidget))
	UImage* HealthBarImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName ProgressName = "ProgressAlpha";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BindWidget))
	UAREffectContainer* EffectContainer;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicHealthBar;
};
