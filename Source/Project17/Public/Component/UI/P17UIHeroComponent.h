// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P17UIPawnComponent.h"
#include "P17UIHeroComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP17OnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FP17OnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17UIHeroComponent : public UP17UIPawnComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17UIHeroComponent();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP17OnPercentChangedDelegate OnRageChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FP17OnEquippedWeaponChangedDelegate OnWeaponChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FP17OnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;
};
