// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARMagicProjectile.generated.h"

class AARCharacter;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class GAMESANDBOX_API AARMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	AARMagicProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void AddActorToIgnore(AActor* Actor);
	void SetTarget(AActor* TheTarget);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Damage")
	float Damage = 20.f;

private:
	UPROPERTY()
	AActor* Target;

};
