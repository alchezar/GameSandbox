// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17GameplayAbility.h"
#include "P17EnemyGameplayAbility.generated.h"

class UP17CombatEnemyComponent;
class AP17CharacterEnemy;

UCLASS()
class PROJECT17_API UP17EnemyGameplayAbility : public UP17GameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	AP17CharacterEnemy* GetEnemyCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	UP17CombatEnemyComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InEffectClass, const FScalableFloat& InScalableDamage) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	TWeakObjectPtr<AP17CharacterEnemy> CachedEnemyCharacter = nullptr;
	TWeakObjectPtr<UP17CombatEnemyComponent> CachedEnemyCombatComponent = nullptr;
};
