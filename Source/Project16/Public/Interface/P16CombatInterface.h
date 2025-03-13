// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CombatInterface.generated.h"

UINTERFACE(BlueprintType)
class UP16CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16CombatInterface
{
	GENERATED_BODY()

public:
	virtual int32   GetPlayerLevel() { return 0; }
	virtual FVector GetCombatSocketLocation() { return FVector::ZeroVector; }
	virtual void    Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTo(const FVector& Target);
};
