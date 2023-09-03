// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P10Powerup.h"
#include "P10RegenerateHealth.generated.h"

class UP10HealthComponent;

UCLASS()
class GAMESANDBOX_API AP10RegenerateHealth : public AP10Powerup
{
	GENERATED_BODY()

public:
	AP10RegenerateHealth();

protected:
	virtual void BeginPlay() override;
	virtual void OnActivated(AActor* Target) override;
	virtual void OnTickPowerup() override;

protected:
	UPROPERTY()
	AActor* TargetActor = nullptr;
	UPROPERTY()
	UP10HealthComponent* HealthComponent = nullptr;
	float HealthDelta = 0.f;
	float Heal = 0.f;
};
