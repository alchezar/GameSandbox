// Copyright (C) 2023, IKinder

#include "P10/Public/AI/P10TrackerBot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Util/P10Library.h"

AP10TrackerBot::AP10TrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
}

void AP10TrackerBot::BeginPlay()
{
	Super::BeginPlay();

	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	LastFrameLocation = MeshComp->GetComponentLocation();
	FindNextPathPoint(TargetPawn);
}

void AP10TrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToNextPoint();
}

void AP10TrackerBot::FindNextPathPoint(AActor* Goal)
{
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, MeshComp->GetComponentLocation(), Goal);
	if (!NavPath || NavPath->PathPoints.Num() < 1)
	{
		NextPathPoint = MeshComp->GetComponentLocation();
		return;
	}

	const FVector NextPoint = NavPath->PathPoints[1];
	NextPathPoint = NextPoint;
}

void AP10TrackerBot::MoveToNextPoint()
{
	/* Check the distance to the target, if we are close enough to it - find the next target. */
	const FVector CurrentLocation = MeshComp->GetComponentLocation();
	const float DistanceToTarget = FVector::Dist(NextPathPoint, CurrentLocation);
	if (DistanceToTarget < AcceptableDistance)
	{
		FindNextPathPoint(TargetPawn);
		return;
	}

	/* Find the angle between current move and target directions, which will proportionally affect the miss compensation force. */
	const FVector MoveDirection = (CurrentLocation - LastFrameLocation).GetSafeNormal2D();
	const FVector TargetDirection = (NextPathPoint - CurrentLocation).GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MoveDirection, TargetDirection)));
	const float MissCompensation = FMath::GetMappedRangeValueClamped(FVector2D(0.f ,180.f), FVector2D(1.f, MaxMissCompensation), Angle);

	/* To correct the motion force - invert current move direction if we`ve already missed the target, or find reflection vector if we haven`t. */
	const FVector ResistDirection = (Angle > 90.f) ? (-1.f * MoveDirection) : (FMath::GetReflectionVector(-1.f * MoveDirection, TargetDirection));
	const FVector FinalDirection = (TargetDirection + ResistDirection).GetSafeNormal2D();

	MeshComp->AddForce(FinalDirection * MissCompensation * Force, NAME_None, bVelocityChange);
	LastFrameLocation = CurrentLocation;

	if (UP10Library::GetIsDrawDebugAllowed() && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Miss compensation: %f"), MissCompensation));
		DrawDebugCapsule(GetWorld(), NextPathPoint, 20.f, 20.f, FQuat::Identity, FColor::Blue, false, -1.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + MoveDirection   * 150.f, 10.f, FColor::Red,    false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + TargetDirection * 150.f, 10.f, FColor::Blue,   false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + ResistDirection * 150.f, 10.f, FColor::Yellow, false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + FinalDirection  * 150.f, 10.f, FColor::Green,  false, -1, 0, 5.f);
	}
}
