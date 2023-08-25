// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Weapon.generated.h"

class UNiagaraSystem;

UCLASS()
class GAMESANDBOX_API AP10Weapon : public AActor
{
	GENERATED_BODY()

public:
	AP10Weapon();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE USkeletalMeshComponent* GetWeaponComponent() const { return WeaponMeshComponent; }
	FORCEINLINE FName GetFirstSocketName() const { return WeaponMeshComponent->GetAllSocketNames()[0]; }
	void StartFire();

protected:
	virtual void BeginPlay() override;

private:
	void  DrawDebugShoot(const FHitResult& OutHit, const FVector& Start, const FVector& End) const; 

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USceneComponent* RootOffsetComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	FVector LocationOffset = FVector(-1.f, -10.f, -1.f);
	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	FRotator RotationOffset = FRotator(5.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* FireEffect;
};
