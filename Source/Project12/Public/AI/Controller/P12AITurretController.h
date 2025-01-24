// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12AIController.h"
#include "P12AITurretController.generated.h"

class AP12Turret;

UCLASS()
class PROJECT12_API AP12AITurretController : public AP12AIController
{
	GENERATED_BODY()

public:
	AP12AITurretController();
	virtual void SetPawn(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AP12Turret> CachedTurret;
};
