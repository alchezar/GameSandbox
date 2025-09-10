// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "P17WeaponBase.h"
#include "Util/P17Types.h"
#include "P17WeaponHero.generated.h"

UCLASS()
class PROJECT17_API AP17WeaponHero : public AP17WeaponBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17WeaponHero();

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintPure, Category = "C++ | Data")
	FP17WeaponData GetHeroWeaponData() const { return Data; };

	UFUNCTION(BlueprintPure, Category = "C++ | Data")
	TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandles();

	// clang-format off
	UFUNCTION(BlueprintCallable, Category = "C++ | Data")
	void SetGrantedAbilitySpecHandles(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);
	// clang-format on

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Data")
	FP17WeaponData Data = {};

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles = {};
};
