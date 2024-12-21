// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/ARProjectileBase.h"
#include "ARProjectileMagic.generated.h"

class UARAbilityEffect;

UCLASS()
class PROJECT06_API AARProjectileMagic : public AARProjectileBase
{
	GENERATED_BODY()

public:
	AARProjectileMagic();
	virtual void Tick(float DeltaTime) override;
	virtual void AddActorToIgnore(AActor* Actor) override;
	
	void SetTarget(AActor* TheTarget);

protected:
	virtual void BeginPlay() override;
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void Explode() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Effect")
	TSubclassOf<UARAbilityEffect> BurningEffectClass;
	
private:
	UPROPERTY()
	AActor* Target;
};
