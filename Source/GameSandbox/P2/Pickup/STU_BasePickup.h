// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_BasePickup.generated.h"

class USoundCue;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BasePickup : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BasePickup();
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	virtual bool TryToGivePickup(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Pickup")
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, Category = "Kinder | Pickup", meta = (Units = "s"))
	float RespawnTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* PickupSound;

private:
	FTimerHandle RespawnTimer;
	double RotationYaw = 1.5;
};
