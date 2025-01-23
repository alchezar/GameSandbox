// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P9DestructibleProp.generated.h"

class UGeometryCollectionComponent;
class UBoxComponent;

UCLASS()
class PROJECT09_API AP9DestructibleProp : public AActor
{
	GENERATED_BODY()

public:
	AP9DestructibleProp();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void Destruct(const float Damage, const FVector& HitLocation, const FVector& ImpulseDirection, const float Impulse);

	UFUNCTION()
	void OnDestructibleHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Destructable")
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Destructable")
	UBoxComponent* TriggerComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Destructable")
	UGeometryCollectionComponent* DestructibleComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Destructable")
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Destructable")
	float CurrentHealth = 0.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Destructable")
	float DefaultDamage = 1.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Destructable")
	float DefaultImpulse = 1.f;
	
private:
	bool bDestroyed = false;
	bool bTriggerEnabled = true;
};
