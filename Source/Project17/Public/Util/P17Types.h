// Copyright © 2025, Ivan Kinder

#pragma once
#include "GameplayTagContainer.h"

#include "P17Types.generated.h"

class UP17HeroGameplayAbility;
class UInputMappingContext;
class UP17LinkedAnimLayer;

USTRUCT(BlueprintType)
struct FP17HeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UP17HeroGameplayAbility> Ability = {};

public:
	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FP17WeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UP17LinkedAnimLayer> AnimLayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TObjectPtr<UInputMappingContext> Context = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FP17HeroAbilitySet> Abilities = {};
};
