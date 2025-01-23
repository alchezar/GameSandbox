// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MainWidget.generated.h"

class AP11Character;
class UTextBlock;

UCLASS()
class PROJECT11_API UP11MainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnHealthChangedHandle(const float Health);
	UFUNCTION()
	void OnAmmoChangedHandle(const int32 Ammo);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

private:
	UPROPERTY()
	AP11Character* CurrentCharacter;
};
