// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12PlayerHUDWidget.generated.h"

class UTextBlock;
class AP12BaseCharacter;
class UProgressBar;

UCLASS()
class GAMESANDBOX_API UP12PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void OnHealthChangeHandle(const float Health, const float MaxHealth);
	void OnAimStateChangedHandle(const bool bStart);
	void OnAmmoCountChangedHandle(const int32 Ammo);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ReticleAiming;

private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;	
};
