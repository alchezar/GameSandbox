// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P10Powerup.h"
#include "P10RegenerateHealth.generated.h"

class UP10HealthComponent;

UCLASS()
class PROJECT10_API AP10RegenerateHealth : public AP10Powerup
{
	GENERATED_BODY()

public:
	AP10RegenerateHealth();

protected:
	virtual void OnActivated(AActor* Target) override;
	virtual void OnTickPowerup() override;

	UPROPERTY()
	AActor* TargetActor = nullptr;
	UPROPERTY()
	UP10HealthComponent* HealthComponent = nullptr;
	float HealthDelta = 0.f;
	float Heal = 0.f;
};
