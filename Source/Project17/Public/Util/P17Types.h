// Copyright © 2025, Ivan Kinder

#pragma once
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "P17Types.generated.h"

class UP17HeroGameplayAbility;
class UInputMappingContext;
class UP17LinkedAnimLayer;

UENUM()
enum class EP17ConfirmTypePin : uint8
{
	Yes,
	No
};

UENUM()
enum class EP17ValidTypePin : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EP17SuccessTypePin : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EP17CountDownInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EP17CountDownOutput : uint8
{
	Updated,
	Completed,
	Canceled
};

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
struct FP17HeroSpecialAbilitySet : public FP17HeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = {};
};

USTRUCT(BlueprintType)
struct FP17WeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UP17LinkedAnimLayer> AnimLayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> Context = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FP17HeroAbilitySet> Abilities = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "CooldownTag"))
	TArray<FP17HeroSpecialAbilitySet> SpecialAbilities = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat BaseDamage = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftIconTexture = nullptr;
};

USTRUCT(BlueprintType)
struct FP17Timespan
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Remaining = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration = 0.f;
};
