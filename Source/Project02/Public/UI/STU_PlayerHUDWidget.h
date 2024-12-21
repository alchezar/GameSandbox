// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWidget.h"
#include "Game/STU_CoreTypes.h"
#include "STU_PlayerHUDWidget.generated.h"

class UProgressBar;

UCLASS()
class PROJECT02_API USTU_PlayerHUDWidget : public USTU_BaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	bool GetCurrentAmmoData(FAmmoData& AmmoData) const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	bool IsPlayerAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	bool IsPlayerSpectating() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	FString FormatBullet(int32 BulletNum);

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Kinder")
	void OnTakeDamage();

private:
	template <class T>
	T* GetOwnerComponent() const;

	void OnHealthChangeHandle(float Health, float HealthDelta);
	void OnNewPawnHandle(APawn* NewPawn);
	void UpdateHealthBar();

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar", meta = (Units = "%"))
	float VisibilityThreshold = 0.8f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar", meta = (Units = "%"))
	float ColorThreshold = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar")
	FLinearColor GoodColor = FLinearColor(0.0f, 0.5f, 0.2f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar")
	FLinearColor BadColor = FLinearColor(1.f, 0.1f, 0.1f);
};
