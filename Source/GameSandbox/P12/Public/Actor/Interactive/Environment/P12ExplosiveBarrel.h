// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12ExplosiveBarrel.generated.h"

class UP12ExplosionComponent;

UCLASS()
class GAMESANDBOX_API AP12ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	AP12ExplosiveBarrel();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void OnExplosionHandle();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UStaticMeshComponent* BarrelMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UP12ExplosionComponent* ExplosionComponent;
	
};
