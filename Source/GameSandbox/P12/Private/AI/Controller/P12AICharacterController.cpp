// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Controller/P12AICharacterController.h"

#include "P12/Public/AI/Character/P12AICharacter.h"
#include "Perception/AISense_Sight.h"

AP12AICharacterController::AP12AICharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP12AICharacterController::BeginPlay()
{
	Super::BeginPlay();
}

void AP12AICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!InPawn)
	{
		CachedAICharacter = nullptr;
		return;
	}

	check(InPawn->IsA<AP12AICharacter>())
	CachedAICharacter = StaticCast<AP12AICharacter*>(InPawn);
}

void AP12AICharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12AICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedAICharacter.IsValid())
	{
		return;
	}
	if (AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass()))
	{
		MoveToActor(ClosestActor);
	}
	
}

