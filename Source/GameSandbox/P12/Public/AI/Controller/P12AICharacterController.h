// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12AIController.h"
#include "P12AICharacterController.generated.h"

class AP12AICharacter;

UCLASS()
class GAMESANDBOX_API AP12AICharacterController : public AP12AIController
{
	GENERATED_BODY()

public:
	AP12AICharacterController();
	virtual void SetPawn(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AP12AICharacter> CachedAICharacter;
};
