// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Environment/P12PlatformBase.h"

#include "P12/Public/Actor/Interactive/Environment/P12PlatformTrigger.h"

AP12PlatformBase::AP12PlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformStaticMeshComponent");
	SetRootComponent(PlatformMesh);
}

void AP12PlatformBase::BeginPlay()
{
	Super::BeginPlay();
	SetupTimeline();

	if (PlatformBehavior == EP12PlatformBehavior::Loop)
	{
		NewPlatformMove();
	}
	StartLocation = GetActorLocation();
	EndLocation = GetActorTransform().TransformPosition(PlatformEndLocation);

	if (PlatformTrigger)
	{
		PlatformTrigger->OnTriggerActivated.AddUObject(this, &ThisClass::OnTriggerActiveHandle);
	}
}

void AP12PlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlatformTimeline.TickTimeline(DeltaTime);
}

void AP12PlatformBase::NewPlatformMove()
{
	if (bMovingForward)
	{
		PlatformTimeline.Reverse();
		bMovingForward = false;
	}
	else
	{
		PlatformTimeline.Play();
		bMovingForward = true;
	}
}

void AP12PlatformBase::SetupTimeline()
{
	FOnTimelineFloat OnTimelineUpdate;
	OnTimelineUpdate.BindDynamic(this, &ThisClass::PlatformUpdateHandle);
	PlatformTimeline.AddInterpFloat(MovementCurve, OnTimelineUpdate);

	FOnTimelineEvent OnTimelineFinish;
	OnTimelineFinish.BindDynamic(this, &ThisClass::PlatformFinishHandle);
	PlatformTimeline.SetTimelineFinishedFunc(OnTimelineFinish);
}

void AP12PlatformBase::OnTriggerActiveHandle(bool bActive)
{
	NewPlatformMove();
}

void AP12PlatformBase::PlatformUpdateHandle(float Output)
{
	const FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Output);
	SetActorLocation(NewLocation);
}

void AP12PlatformBase::PlatformFinishHandle()
{
	if (PlatformBehavior == EP12PlatformBehavior::Loop)
	{
		NewPlatformMove();
		return;
	}
	if (ReturnTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ReturnTimer, this, &ThisClass::NewPlatformMove, ReturnTime, false);
	}
}
