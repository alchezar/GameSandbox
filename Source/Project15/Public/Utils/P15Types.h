// Copyright © 2025, Ivan Kinder

#pragma once

#include "P15Types.generated.h"

class UP15BaseAbility;

UENUM(BlueprintType)
enum class EP15AbilityCostType : uint8
{
	Health,
	Mana,
	Strength
};

struct FP15SmoothChangeData
{
	bool  bActive   = false;
	bool  bPositive = false;
	float Time      = 0.5f;
	float Alpha     = 0.f;
};

USTRUCT(BlueprintType)
struct FP15AbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownDuration = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EP15AbilityCostType CostType = EP15AbilityCostType::Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Texture = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UP15BaseAbility> AbilityClass = nullptr;

	static FP15AbilityInfo Default()
	{
		return FP15AbilityInfo{};
	}

	FP15AbilityInfo& SetCooldown(const float NewCooldown)
	{
		CooldownDuration = NewCooldown;
		return *this;
	}

	FP15AbilityInfo& SetCost(const float NewCost)
	{
		Cost = NewCost;
		return *this;
	}

	FP15AbilityInfo& SetCostType(const EP15AbilityCostType NewCostType)
	{
		CostType = NewCostType;
		return *this;
	}

	FP15AbilityInfo& SetTexture(UTexture2D* NewTexture)
	{
		Texture = NewTexture;
		return *this;
	}

	FP15AbilityInfo& SetAbilityClass(const TSubclassOf<UP15BaseAbility>& NewAbilityClass)
	{
		AbilityClass = NewAbilityClass;
		return *this;
	}
};
