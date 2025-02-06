// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P15PrimaryWidget.generated.h"

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
protected:
	void OnHealthChangedCallback(const float NewHealthPercentage);
	void OnManaChangedCallback(const float NewManaPercentage);
	void OnStrengthChangedCallback(const float NewStrengthPercentage);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* HealthBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* ManaBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15GlassBallWidget* StrengthBallWidget;
	UPROPERTY(Transient, meta = (BindWidget))
	UP15AbilitySlotWidget* DashAbilitySlotWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName BallLevelParameterName = "Level";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName SlotAlphaParameterName = "Alpha";
};
