// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12PlayerHUDWidget.generated.h"

class UTextBlock;
class AP12BaseCharacter;
class UProgressBar;

UCLASS()
class PROJECT12_API UP12PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCachedCharacter(AP12BaseCharacter* NewCharacter);

protected:
	virtual void NativeConstruct() override;
	void OnHealthChangeHandle(const float Health, const float MaxHealth);
	void OnAmmoCountChangedHandle(const int32 Ammo, const int32 TotalAmmo);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalAmmoText;

private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
};
