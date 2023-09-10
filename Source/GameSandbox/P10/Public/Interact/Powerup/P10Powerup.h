// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Powerup.generated.h"

UCLASS()
class GAMESANDBOX_API AP10Powerup : public AActor
{
	GENERATED_BODY()

public:
	AP10Powerup();
	virtual void ActivatePowerup(AActor* Target);
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnTickPowerup();
	virtual void OnActivated(AActor* Target);
	virtual void OnExpired();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnActivated(AActor* Target);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnExpired();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, Category = "C++", meta = (Units = "s"))
	float MeshFullRotationTime = 4.f;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Steps")
	float Interval = 0.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Steps")
	int32 TotalTicks = 0;
	
	FTimerHandle PowerupTimer;
};
