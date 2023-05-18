// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG_Gun.generated.h"

class ATG_BaseCharacter;
class ATG_Projectile;
class ATG_FirstPersonCharacter;
class UTG_AN_Fire;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFiringSignature, bool)

UCLASS()
class GAMESANDBOX_API ATG_Gun : public AActor
{
	GENERATED_BODY()

public:
	ATG_Gun();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	void StartFire();
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	void StopFire();

	float GetWeaponDamage() const;
	void  SetAnimInstance(UAnimInstance* NewAnimInstance);
	void  SetWeaponOwner(ATG_BaseCharacter* TheOwner);

protected:
	virtual void BeginPlay() override;
	void         OnFire();

private:
	void InitAnimation();
	void OnFireAnimStarted(USkeletalMeshComponent* MeshComp);

public:
	FOnFiringSignature OnFiring;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	USkeletalMeshComponent* FP_Gun;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	USceneComponent* FP_Muzzle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	TSubclassOf<ATG_Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	TArray<UAnimSequenceBase*> OwnerFireAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	float Damage = 10.f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	// float TimeBetweenShoots = 0.1f; 

private:
	// FTimerHandle FireTimer;

	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY()
	ATG_BaseCharacter* WeaponOwner;
};
