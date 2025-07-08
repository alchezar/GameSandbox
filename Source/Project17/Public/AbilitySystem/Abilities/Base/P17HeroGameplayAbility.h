// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17GameplayAbility.h"
#include "P17HeroGameplayAbility.generated.h"

class UP17CombatHeroComponent;
class AP17ControllerHero;
class AP17CharacterHero;

UCLASS()
class PROJECT17_API UP17HeroGameplayAbility : public UP17GameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	AP17CharacterHero* GetHeroCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	AP17ControllerHero* GetHeroControllerFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	UP17CombatHeroComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InEffectClass, const float InDamage, const FGameplayTag InAttackTag, const int32 InComboCount) const;

protected:
	void FaceControllerTo(const AActor* Target, const float DeltaTime, const float InterpSpeed) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	TWeakObjectPtr<AP17CharacterHero> CachedHeroCharacter = nullptr;
	TWeakObjectPtr<AP17ControllerHero> CachedHeroController = nullptr;
	TWeakObjectPtr<UP17CombatHeroComponent> CachedHeroCombatComponent = nullptr;
};
