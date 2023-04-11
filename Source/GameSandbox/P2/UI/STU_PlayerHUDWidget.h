// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "STU_PlayerHUDWidget.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_PlayerHUDWidget : public UUserWidget
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

private:
	template <class T>
	T* GetOwnerComponent() const;
};
