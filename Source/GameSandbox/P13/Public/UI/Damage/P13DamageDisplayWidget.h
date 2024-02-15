// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13/Public/Component/Scene/P13DamageDisplayComponent.h"
#include "P13DamageDisplayWidget.generated.h"

class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP13DamageDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) override;

	/* ------------------------------- This -------------------------------- */
public:
	void SetupDamageWidget(const USceneComponent* ComponentToAttach, const float CurrentDamage, const float NewHealthAlpha, const FVector2D NewRandomOffset);
	void SetupShieldWidget(const USceneComponent* ComponentToAttach, const FString& ShieldString, const FVector2D NewRandomOffset, const FLinearColor& NewColor = FLinearColor::Blue);

private:
	void UpdateScreenLocation();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LiftAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UCurveLinearColor* ColorOverLife;
	UPROPERTY(EditAnywhere, Category = "C++")
	float ShowTime = 0.5f;

private:
	TSoftObjectPtr<USceneComponent> CachedComponentToAttach;
	float HealthAlpha = 0.f;
	float Lifetime = 0.f;
	FVector2D RandomOffset = FVector2D::Zero();
	FLinearColor DefaultColor = FLinearColor::White;
};
