// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AREffectContainer.generated.h"

class UAREffectSlot;
class UHorizontalBox;
class UARAbility;
class UARAbilityComponent;

UCLASS()
class PROJECT06_API UAREffectContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	void SetAbilityComponent(UARAbilityComponent* AbilityComp);
	
protected:
	UFUNCTION()
	void OnAbilityStartedHandle(UARAbilityComponent* AbilityComp, UARAbility* Ability);

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* EffectBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UAREffectSlot> EffectSlotClass;
	
private:
	UPROPERTY()
	UARAbilityComponent* AbilityComponent;
};
