// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Base/P17HeroGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "P17TraceableAbility.generated.h"

UCLASS()
class PROJECT17_API UP17TraceableAbility : public UP17HeroGameplayAbility
{
	GENERATED_BODY()

protected:
	TArray<FHitResult> BoxTrace() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	float TraceDistance = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	FVector TraceBoxSize = FVector {300.f};
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceChannels = {};
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	bool bShowDebug = false;
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace", meta = (EditCondition = "bShowDebug"))
	bool bPersistent = false;
};
