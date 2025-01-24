// Copyright (C) 2023, IKinder

#include "Component/Actor/P12AIPatrollingComponent.h"

#include "Actor/Navigation/P12PatrollingPath.h"

FVector UP12AIPatrollingComponent::SelectClosestWayPoint()
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector>& WayPoints = PatrollingPath->GetWayPoints();
	const FTransform PathTransform = PatrollingPath->GetActorTransform();

	float MinDistance = FLT_MAX;
	FVector ClosestWayPoint;
	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		const FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		const float CurrentDistance = FVector::DistSquared(OwnerLocation, WayPointWorld);
		if (CurrentDistance > MinDistance)
		{
			continue;
		}
		ClosestWayPoint = WayPointWorld;
		MinDistance = CurrentDistance;
		CurrentWayPointIndex = i;
	}
	return ClosestWayPoint;
}

FVector UP12AIPatrollingComponent::SelectNextWayPoint()
{
	const TArray<FVector>& WayPoints = PatrollingPath->GetWayPoints();
	const FTransform PathTransform = PatrollingPath->GetActorTransform();

	CurrentWayPointIndex = (CurrentWayPointIndex + 1) % WayPoints.Num();
	return PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
}

bool UP12AIPatrollingComponent::GetCanPatrol() const
{
	return PatrollingPath && !PatrollingPath->GetWayPoints().IsEmpty();
}
