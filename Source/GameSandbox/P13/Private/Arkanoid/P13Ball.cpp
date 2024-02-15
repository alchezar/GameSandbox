// Copyright © 2024, IKinder

#include "P13/Public/Arkanoid/P13Ball.h"

#include "GameFramework/KillZVolume.h"

AP13Ball::AP13Ball()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>("RootStaticMeshComponent");
	SetRootComponent(RootMesh);
}

void AP13Ball::BeginPlay()
{
	Super::BeginPlay();
}

void AP13Ball::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveBall(DeltaTime);
}

void AP13Ball::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	TryToDestroyBall(OtherActor);
}

void AP13Ball::MoveBall(const float DeltaTime)
{
	const FVector DeltaLocation = FVector(Direction * Speed * DeltaTime);
	FHitResult HitResult;
	AddActorWorldOffset(DeltaLocation, true, &HitResult);

	if (HitResult.bBlockingHit)
	{
		Direction = FMath::GetReflectionVector(Direction, HitResult.Normal);
		Direction.Z = 0.f;
	}
}

void AP13Ball::AddDelegateHandle(const FDelegateHandle InDeadDelegateHandle)
{
	DeadDelegateHandle = InDeadDelegateHandle;
}

void AP13Ball::TryToDestroyBall(const AActor* OtherActor)
{
	if (!OtherActor->IsA<AKillZVolume>())
	{
		return;
	}
	OnBallDead.Broadcast();
	OnBallDead.Remove(DeadDelegateHandle);
	Destroy();
}
