// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P12AIController.generated.h"

class UAISense;

UCLASS()
class GAMESANDBOX_API AP12AIController : public AAIController
{
	GENERATED_BODY()

public:
	AP12AIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	AActor* GetClosestSensedActor(const TSubclassOf<UAISense> SenseClass) const;
};
