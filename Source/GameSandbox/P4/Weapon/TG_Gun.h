// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG_Gun.generated.h"

class ATG_Projectile;
class ATG_FirstPersonCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFiringSignature, bool)

UCLASS()
class GAMESANDBOX_API ATG_Gun : public AActor
{
	GENERATED_BODY()

public:
	ATG_Gun();
	virtual void Tick(float DeltaTime) override;
	void StartFire();
	void StopFire();

	void SetAnimInstance(UAnimInstance* NewAnimInstance);

protected:
	virtual void BeginPlay() override;
	void OnFire();

public:
	FOnFiringSignature OnFiring;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	USkeletalMeshComponent* FP_Gun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	USceneComponent* FP_Muzzle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Mesh")
	TSubclassOf<ATG_Projectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Gameplay")
	float TimeBetweenShoots = 0.1f; 

private:
	FTimerHandle FireTimer;
};
