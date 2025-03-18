// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16CharacterClassInfoDataAsset.generated.h"

class UGameplayEffect;

UCLASS()
class PROJECT16_API UP16CharacterClassInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FP16CharacterClassDefaultInfo GetClassDefaultInfo(const EP16CharacterClass CharacterClass);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities = {};
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Common Class Defaults | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Class Defaults")
	TMap<EP16CharacterClass, FP16CharacterClassDefaultInfo> CharacterClassInfoMap = {};
};
