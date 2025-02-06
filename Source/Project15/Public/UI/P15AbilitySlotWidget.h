// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P15AbilitySlotWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECT15_API UP15AbilitySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	void SetAbilityCost(const float NewCost) const;
	void SetCooldown(const float NewCooldown);
	void UpdateCooldown(const float NewCooldown) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UImage> AbilityImage;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UMaterialInterface> ImageMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName TextureParameterName = "Texture";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName ColorParameterName = "Color";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName AlphaParameterName = "Alpha";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FLinearColor AbilityColor = FLinearColor::Green;

private:
	float MaxCooldown = 1.f;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;
};
