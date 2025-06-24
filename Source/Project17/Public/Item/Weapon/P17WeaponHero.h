// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
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

public:
	UFUNCTION(BlueprintPure, Category = "C++ | Data")
	FP17WeaponData GetHeroWeaponData() const { return Data; };

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Data")
	FP17WeaponData Data = {};
};
