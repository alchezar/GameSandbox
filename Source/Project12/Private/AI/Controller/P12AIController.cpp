// Copyright (C) 2023, IKinder

#include "AI/Controller/P12AIController.h"

#include "Perception/AIPerceptionComponent.h"

AP12AIController::AP12AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

void AP12AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AP12AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* AP12AIController::GetClosestSensedActor(const TSubclassOf<UAISense> SenseClass) const
{
	const APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return nullptr;
	}

	TArray<AActor*> SensedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	const FVector PawnLocation = ControlledPawn->GetActorLocation();
	float MinSquaredDistance = FLT_MAX;
	AActor* ClosestActor = nullptr;

	for (AActor* SensedActor : SensedActors)
	{
		if (SensedActor->GetLifeSpan() != 0.f)
		{
			continue;
		}
		const float DistanceToActor = FVector::DistSquared(PawnLocation, SensedActor->GetActorLocation());
		if (DistanceToActor >= MinSquaredDistance)
		{
			continue;
		}
		MinSquaredDistance = DistanceToActor;
		ClosestActor = SensedActor;
	}
	return ClosestActor;
}
