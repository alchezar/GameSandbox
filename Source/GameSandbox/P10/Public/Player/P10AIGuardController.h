// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P10AIGuardController.generated.h"

UCLASS()
class GAMESANDBOX_API AP10AIGuardController : public AAIController
{
	GENERATED_BODY()

public:
	AP10AIGuardController();
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	virtual void BeginPlay() override;
};
