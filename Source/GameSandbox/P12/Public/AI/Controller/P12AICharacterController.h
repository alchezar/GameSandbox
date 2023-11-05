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
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	virtual void BeginPlay() override;

private:
	void MoveToNextTarget();
	bool GetIsTargetReached(const FVector& TargetLocation);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	float TargetReachRadius = 100.f;
	
private:
	TWeakObjectPtr<AP12AICharacter> CachedAICharacter;
	bool bPatrolling = false;
};
