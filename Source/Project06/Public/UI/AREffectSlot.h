// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AREffectSlot.generated.h"

class UImage;
class UARAbilityEffect;

UCLASS()
class PROJECT06_API UAREffectSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void SetAbilityEffect(UARAbilityEffect* Effect);

protected:
	UFUNCTION()
	void OnAbilityStoppedHandle(UARAbilityComponent* AbilityComp, UARAbility* Ability);

private:
	void ChangeEffectIcon();
	void BindDelegate();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* EffectImage;

private:
	UPROPERTY()
	UARAbilityEffect* AbilityEffect;
};
