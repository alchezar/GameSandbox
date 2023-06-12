// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARAbility.h"
#include "ARAbilityProjectileBase.generated.h"

class UParticleSystem;
class UAnimMontage;

UCLASS()
class GAMESANDBOX_API UARAbilityProjectileBase : public UARAbility
{
	GENERATED_BODY()

public:
	UARAbilityProjectileBase();
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void SpawnProjectile(ACharacter* Instigator, const bool bHoming, const TSubclassOf<AActor> ClassToSpawn, const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Effect")
	UParticleSystem* CastingEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Effect")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Effect")
	bool bHomingProjectile = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Body")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	UAnimMontage* AttackMontage;
};
