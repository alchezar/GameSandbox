// Copyright (C) 2023, IKinder

#include "AI/Controller/P12AITurretController.h"

#include "AI/Character/P12Turret.h"
#include "Perception/AISense_Sight.h"

AP12AITurretController::AP12AITurretController()
{
	PrimaryActorTick.bCanEverTick = true;
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
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedTurret.IsValid())
	{
		return;
	}

	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	CachedTurret->SetCurrentTarget(ClosestActor);
}
