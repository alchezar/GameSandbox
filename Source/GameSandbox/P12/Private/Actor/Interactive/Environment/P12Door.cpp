// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Environment/P12Door.h"

AP12Door::AP12Door()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRootComponent"));

	DoorPivot = CreateDefaultSubobject<USceneComponent>("DoorPivotSceneComponent");
	DoorPivot->SetupAttachment(RootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorStaticMeshComponent");
	DoorMesh->SetupAttachment(DoorPivot);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("FrameStaticMeshComponent");
	FrameMesh->SetupAttachment(RootComponent);
	
}

void AP12Door::BeginPlay()
{
	Super::BeginPlay();

	if (DoorCurve)
	{
		FOnTimelineFloat UpdateTimeline;
		UpdateTimeline.BindDynamic(this, &ThisClass::OnUpdateDoorTimelineHandle);
		DoorOpenTimeline.AddInterpFloat(DoorCurve, UpdateTimeline);

		FOnTimelineEvent FinishTimeline;
		FinishTimeline.BindDynamic(this, &ThisClass::OnFinishedDoorTimelineHandle);
		DoorOpenTimeline.SetTimelineFinishedFunc(FinishTimeline);
	}
}

void AP12Door::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoorOpenTimeline.TickTimeline(DeltaTime);
}

void AP12Door::Interact(AP12BaseCharacter* Char)
{
	if (!bCustomCurve || !DoorCurve)
	{
		OpenDoor();
		return;
	}
	check(DoorCurve)
	InteractWithDoor();
}

FName AP12Door::GetActionEventName() const
{
	return FName("Interact_Open");
}

void AP12Door::InteractWithDoor() 
{
	if (DoorOpenTimeline.IsPlaying())
	{
		return;
	}
	SetActorTickEnabled(true);
	DoorOpenTimeline.PlayFromStart();
}

void AP12Door::OnUpdateDoorTimelineHandle(float TimelineAlpha) 
{
	const float StartAngle = bOpened ? Angle.Opened : Angle.Closed;
	const float EndAngle = bOpened ? Angle.Closed : Angle.Opened;
	const float Yaw = FMath::Lerp(StartAngle, EndAngle, FMath::Clamp(TimelineAlpha, 0.f, 1.f));
	DoorPivot->SetRelativeRotation(FRotator(0.f, Yaw, 0.f));
}

void AP12Door::OnFinishedDoorTimelineHandle() 
{
	bOpened = !bOpened;
	SetActorTickEnabled(false);
}

void AP12Door::OpenDoor()
{
	bOpened = !bOpened;
	SetActorTickEnabled(true);

	/* New Alpha is always the same as previous. If it was 1.f means that previous interaction was completed successfully.
	 * If less - we are trying to interrupt the previous move, so we need as much time as has already passed. */
	Alpha = 1.f - Alpha;

	const FRotator ClosedRotation = FRotator(0.f, Angle.Closed, 0.f);
	const FRotator OpenedRotation = FRotator(0.f, Angle.Opened, 0.f);
	
	const FRotator StartRotation = bOpened ? ClosedRotation : OpenedRotation;
	const FRotator TargetRotation = bOpened ? OpenedRotation : ClosedRotation;
	
	FTimerDelegate DoorDelegate;
	DoorDelegate.BindUObject(this, &ThisClass::OpenDoorSmoothly, StartRotation, TargetRotation);
	GetWorld()->GetTimerManager().SetTimer(DoorTimer, DoorDelegate, GetWorld()->GetDeltaSeconds(), true);	
}

// ReSharper disable CppPassValueParameterByConstReference

void AP12Door::OpenDoorSmoothly(FRotator StartRotation, FRotator TargetRotation)
{
	const float AlphaPerTick = GetWorld()->GetDeltaSeconds() / OpeningTime;
	Alpha = FMath::Clamp(Alpha + AlphaPerTick, 0.f, 1.f);
	
	const float SmoothAlpha = 0.5f - cos(Alpha * UE_PI) / 2.f;
	const FRotator CurrentRotation = FMath::Lerp(StartRotation, TargetRotation, SmoothAlpha);
	DoorPivot->SetRelativeRotation(CurrentRotation);
	
	if (Alpha == 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DoorTimer);
		SetActorTickEnabled(false);
	}
}
