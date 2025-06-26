// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "P17Data_StartupBase.generated.h"

class UGameplayEffect;
class UP17AbilitySystemComponent;
class UP17GameplayAbility;

UCLASS()
class PROJECT17_API UP17Data_StartupBase : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	virtual void GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel = 1);

protected:
	void GrandAbilities(const TArray<TSubclassOf<UP17GameplayAbility>>& InAbilitiesToGive, UP17AbilitySystemComponent* InASC, const int32 InLevel = 1);
	void GrandEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffectsToApply, UP17AbilitySystemComponent* InASC, const int32 InLevel = 1);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Startup")
	TArray<TSubclassOf<UP17GameplayAbility>> ActivateOnGivenAbilities = {};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Startup")
	TArray<TSubclassOf<UP17GameplayAbility>> ReactiveAbilities = {};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Startup")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects = {};
};
