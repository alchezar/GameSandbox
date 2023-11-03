// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Controller/P12AITurretController.h"

#include "P12/Public/AI/Character/P12Turret.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AP12AITurretController::AP12AITurretController()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

void AP12AITurretController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!InPawn)
	{
		CachedTurret = nullptr;
		return;
	}
	
	check(InPawn->IsA<AP12Turret>())
	CachedTurret = StaticCast<AP12Turret*>(InPawn);
}

void AP12AITurretController::BeginPlay()
{
	Super::BeginPlay();
}

void AP12AITurretController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12AITurretController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (!CachedTurret.IsValid())
	{
		return;
	}
	
	TArray<AActor*> SeenActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);

	const FVector TurretLocation = CachedTurret->GetActorLocation();
	float MinSquaredDistance = FLT_MAX;
	AActor* ClosestActor = nullptr;
	
	for (AActor* SeenActor : SeenActors)
	{
		const float DistanceToActor = FVector::DistSquared(TurretLocation, SeenActor->GetActorLocation());
		if (DistanceToActor >= MinSquaredDistance)
		{
			continue;
		}
		MinSquaredDistance = DistanceToActor;
		ClosestActor = SeenActor;
	}
	CachedTurret->SetCurrentTarget(ClosestActor);
}

