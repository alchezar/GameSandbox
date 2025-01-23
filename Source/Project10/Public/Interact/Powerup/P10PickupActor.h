// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10PickupActor.generated.h"

class AP10Powerup;
class USphereComponent;

UCLASS()
class PROJECT10_API AP10PickupActor : public AActor
{
	GENERATED_BODY()

public:
	AP10PickupActor();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	void Respawn();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	TSubclassOf<AP10Powerup> PowerupClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	float Cooldown = 10.f;
	
private:
	FTimerHandle CooldownTimer;
	UPROPERTY()
	AP10Powerup* PowerupInstance;
};
