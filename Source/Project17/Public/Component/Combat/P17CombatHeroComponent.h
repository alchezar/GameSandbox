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
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponHero* GetHeroCarriedWeaponBy(const FGameplayTag InWeaponTag) const;
};
