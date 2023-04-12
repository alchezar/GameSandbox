// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_BasePickup.generated.h"

class USphereComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BasePickup : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BasePickup();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Pickup")
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, Category = "Kinder | Pickup", meta = (Units = "s"))
	float RespawnTime = 5.f;

private:
	virtual bool TryToGivePickup(APawn* PlayerPawn);
	
	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();

	double RotationYaw = 1.5;
};
