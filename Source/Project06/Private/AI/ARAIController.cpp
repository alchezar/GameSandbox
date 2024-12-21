// Copyright (C) 2023, IKinder

#include "AI/ARAIController.h"

AARAIController::AARAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARAIController::BeginPlay()
{
	Super::BeginPlay();
	check(BehaviorTree);

	RunBehaviorTree(BehaviorTree);
}

void AARAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

