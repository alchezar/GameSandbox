// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Util/P16Type.h"
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
	virtual int32 GetPlayerLevel() { return 0; }
	virtual void  Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTo(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FP16TaggedMontage> GetAttackMontages();
};
