// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P16GameplayAbility.generated.h"

/// ----------------------------------------------------------------------------
/// @class   UP16GameplayAbility
/// @brief   Represents a base gameplay ability class.
/// @details Extends the base class with specifying a startup input tag.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	AActor* OrientToTarget();

	virtual FString GetDescription(const int32 CurrentLevel);
	virtual FString GetDescriptionNextLevel(const int32 CurrentLevel);
	static FString  GetDescriptionLocked(const int32 CurrentLevel);

protected:
	_NODISCARD
	float GetManaCost(const float InLevel = 1.f) const;
	_NODISCARD
	float GetCooldownTime(const float InLevel = 1.f) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	FGameplayTag StartupInputTag;
};
