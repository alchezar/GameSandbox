// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16ProjectileSpell.h"
#include "P16Fireball.generated.h"

UCLASS()
class PROJECT16_API UP16Fireball : public UP16ProjectileSpell
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual FString GetDescription(const int32 CurrentLevel) override;
	virtual FString GetDescriptionNextLevel(const int32 CurrentLevel) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& InTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget = nullptr, const float AdditionalPitch = 0.f);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	float ProjectileSpreadAngle = 90.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	int32 MaxNumProjectiles = 5;
};
