// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Util/P16Type.h"
#include "Util/P16Util.h"
#include "P16CombatInterface.generated.h"

class UNiagaraSystem;

UINTERFACE(BlueprintType)
class UP16CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16CombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerLevel();

	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTo(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetIsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetBeingShocked() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FP16TaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FP16TaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(const int32 Amount = 1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EP16CharacterClass GetCharacterClass();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(const bool bInLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBeingShocked(const bool bIsShocked);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	virtual FP16OnAbilitySystemRegisteredSignature& GetOnAbilitySystemRegisteredDelegate() = 0;
	virtual FP16OnDeathSignature&                   GetOnDeathDelegate() = 0;
	virtual FP16OnRadialDamageSignature&            GetOnRadialDamageDelegate() = 0;
};
