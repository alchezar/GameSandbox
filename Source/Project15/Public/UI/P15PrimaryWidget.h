// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/P15Utils.h"
#include "P15PrimaryWidget.generated.h"

struct FP15AbilityInfo;
class UP15AbilitySlotWidget;
class UP15GlassBallWidget;

UCLASS()
class PROJECT15_API UP15PrimaryWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	virtual void NativeOnInitialized() override;

	/* ------------------------------- This -------------------------------- */
public:
	void AddAbilityToUI(const int32 AbilityIndex, FP15AbilityInfo&& AbilityInfo, FP15OnAbilityStartedSignature* OnAbilityStartedDelegate);

protected:
	void OnHealthChangedCallback(const float NewHealthPercentage);
	void OnManaChangedCallback(const float NewManaPercentage);
	void OnStrengthChangedCallback(const float NewStrengthPercentage);
	void OnAbilityStartedCallback(TSubclassOf<UP15BaseAbility> AbilityClass);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* HealthBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* ManaBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* StrengthBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* FirstAbilitySlotWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* SecondAbilitySlotWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* ThirdAbilitySlotWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* FourthAbilitySlotWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* FifthAbilitySlotWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName BallLevelParameterName = "Level";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName SlotAlphaParameterName = "Alpha";

private:
	TArray<UP15AbilitySlotWidget*> AbilitySlots = {};
};
