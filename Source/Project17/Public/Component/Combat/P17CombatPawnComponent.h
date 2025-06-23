// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/P17ExtensionBaseComponent.h"
#include "P17CombatPawnComponent.generated.h"

class AP17WeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17CombatPawnComponent : public UP17ExtensionBaseComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	void RegisterSpawnedWeapon(const FGameplayTag InWeaponTag, AP17WeaponBase* InWeapon, const bool bRegisterAsEquipped = false);
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponBase* GetCarriedWeaponBy(const FGameplayTag InWeaponTag) const;
	UFUNCTION(BlueprintCallable, Category = "C++ | Combat")
	AP17WeaponBase* GetEquippedWeapon() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++ | Combat")
	FGameplayTag EquippedWeaponTag;

private:
	UPROPERTY()
	TMap<FGameplayTag, AP17WeaponBase*> CarriedWeaponMap = {};
};
