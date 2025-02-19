// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/P15Types.h"
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
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD TSubclassOf<UP15BaseAbility> GetAbilityClass() const { return AbilityClass; }

	void SetCooldown(const float NewCooldown);
	void UpdateCooldown(const float NewCooldown) const;
	void SetAbilityInfo(FP15AbilityInfo&& AbilityInfo);
	void OnAbilityStarted();

private:
	_NODISCARD FSlateColor GetCostColor(const EP15AbilityCostType CostType) const;
	void                   UpdateCooldownSmoothly(const float DeltaTime);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UImage> AbilityImage;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UTexture2D> AbilityTexture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName TextureParameterName = "Texture";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName AlphaParameterName = "Alpha";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FLinearColor AbilityColor = FLinearColor::Green;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;

	bool                         bActive      = false;
	float                        MaxCooldown  = 1.f;
	float                        Cooldown     = 0.f;
	TSubclassOf<UP15BaseAbility> AbilityClass = nullptr;
};
