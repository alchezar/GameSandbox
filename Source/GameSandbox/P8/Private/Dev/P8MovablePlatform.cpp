// Copyright (C) 2023, IKinder

#include "P8/Public/Dev/P8MovablePlatform.h"

AP8MovablePlatform::AP8MovablePlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AP8MovablePlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AP8MovablePlatform::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		const FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();
		Location += Direction * Speed * DeltaTime;
		SetActorLocation(Location);
		
		if ((Location - GlobalStartLocation).Size() >= (GlobalTargetLocation - GlobalStartLocation).Size())
		{
			/* Swap variables without creating temp vector */
			GlobalStartLocation += GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation - GlobalTargetLocation;
			GlobalStartLocation  = GlobalStartLocation - GlobalTargetLocation;
		}
	}
}

