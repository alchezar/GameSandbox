// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16GameplayAbility.h"
#include "P16ProjectileSpell.generated.h"

class AP16Projectile;

/// ----------------------------------------------------------------------------
/// @class   UP16ProjectileSpell
/// @brief   Represents a projectile spell ability.
/// @details Extends the base class with an ability to spawn a projectile.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16ProjectileSpell : public UP16GameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Class")
	TSubclassOf<AP16Projectile> ProjectileClass = nullptr;
};
