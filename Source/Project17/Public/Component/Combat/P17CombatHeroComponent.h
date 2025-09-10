// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17CombatPawnComponent.h"
#include "P17CombatHeroComponent.generated.h"

class AP17WeaponHero;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17CombatHeroComponent : public UP17CombatPawnComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void OnHitTargetActorCallback(AActor* HitActor) override;
	virtual void OnWeaponPulledFromActorCallback(AActor* PulledActor) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponHero* GetHeroCarriedWeaponBy(const FGameplayTag InWeaponTag) const;
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponHero* GetHeroEquippedWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	float GetHeroEquippedWeaponDamage(const float InLevel) const;
};
