// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16GameplayAbility.h"
#include "P16SummonAbility.generated.h"

UCLASS()
class PROJECT16_API UP16SummonAbility : public UP16GameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	TArray<FVector> GetSpawnLocations() const;
	UFUNCTION(BlueprintPure, Category = "C++")
	TSubclassOf<APawn> GetRandomMinionClass() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	int32 MinionsNum = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<APawn>> MinionsClasses = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	FVector2D SpawnDistanceRange = {150.f, 400.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	float SpawnSpreadAngle = 90.f;
};
