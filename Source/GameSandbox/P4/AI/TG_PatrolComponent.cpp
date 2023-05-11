// Copyright (C) 2023, IKinder

#include "TG_PatrolComponent.h"

UTG_PatrolComponent::UTG_PatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTG_PatrolComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTG_PatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<ATargetPoint*> UTG_PatrolComponent::GetPatrolPoints() const
{
	return PatrolPoints;
}

