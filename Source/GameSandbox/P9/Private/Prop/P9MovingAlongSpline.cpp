// Copyright (C) 2023, IKinder

#include "P9/Public/Prop/P9MovingAlongSpline.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

AP9MovingAlongSpline::AP9MovingAlongSpline()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(SplineComponent);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComponent");
	TriggerComponent->SetupAttachment(MeshComponent);
}

void AP9MovingAlongSpline::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlapHandle);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTriggerEndOverlapHandle);
}

void AP9MovingAlongSpline::BeginPlay()
{
	Super::BeginPlay();
	check(MovementCurve)

	const FVector SplineLocation = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	MeshComponent->SetWorldLocation(SplineLocation);
	
	FOnTimelineFloat UpdateFunction;
	UpdateFunction.BindDynamic(this, &ThisClass::MovementTimelineUpdateHandle);
	MovementTimeline.AddInterpFloat(MovementCurve, UpdateFunction);

	FOnTimelineEvent FinishedFunction;
	FinishedFunction.BindDynamic(this, &ThisClass::MovementTimelineFinishedHandle);
	MovementTimeline.SetTimelineFinishedFunc(FinishedFunction);

	MovementTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	if (bAutoActive) MovementTimeline.PlayFromStart();
}

void AP9MovingAlongSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying()) MovementTimeline.TickTimeline(DeltaTime);
}

void AP9MovingAlongSpline::OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MovementTimeline.Play();
}

void AP9MovingAlongSpline::OnTriggerEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!MovementTimeline.IsReversing()) MovementTimeline.Reverse();
}

void AP9MovingAlongSpline::MovementTimelineUpdateHandle(float Output)
{
	const float SplineLength = SplineComponent->GetSplineLength();
	const FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Output * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Output * SplineLength, ESplineCoordinateSpace::World);
	CurrentSplineRotation.Pitch = 0.f;

	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
	
}

void AP9MovingAlongSpline::MovementTimelineFinishedHandle()
{
	if (bReverseOnFinished) MovementTimeline.Reverse();
	if (bRestartOnFinished) MovementTimeline.PlayFromStart();
}
