// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARProjectileBase.generated.h"

class AARCharacter;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class GAMESANDBOX_API AARProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AARProjectileBase();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	
	virtual void AddActorToIgnore(AActor* Actor);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Explode();

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Damage")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Effect")
	UParticleSystem* Impact;
};
