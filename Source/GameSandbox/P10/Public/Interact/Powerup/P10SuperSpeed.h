// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P10Powerup.h"
#include "P10SuperSpeed.generated.h"

class UCharacterMovementComponent;

UCLASS()
class GAMESANDBOX_API AP10SuperSpeed : public AP10Powerup
{
	GENERATED_BODY()

public:
	AP10SuperSpeed();

protected:
	virtual void OnActivated(AActor* Target) override;
	virtual void OnExpired() override;
	
private:
	float DefaultMaxWalkSpeed = 0.f;
	UPROPERTY()
	UCharacterMovementComponent* CharMovement = nullptr;
};
