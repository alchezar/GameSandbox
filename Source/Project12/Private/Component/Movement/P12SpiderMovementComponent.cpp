// Copyright (C) 2023, IKinder

#include "Component/Movement/P12SpiderMovementComponent.h"

UP12SpiderMovementComponent::UP12SpiderMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12SpiderMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12SpiderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MovePawn(DeltaTime);
}

bool UP12SpiderMovementComponent::IsFalling() const
{
	return bFalling;
}

void UP12SpiderMovementComponent::JumpStart()
{
	VerticalVelocity = InitJumpVelocity * FVector::UpVector;
	bJump = true;
}

void UP12SpiderMovementComponent::MovePawn(const float DeltaTime)
{
	const FVector PendingInput = GetPendingInputVector().GetClampedToMaxSize(1.0f);
	Velocity = PendingInput * MaxSpeed;
	ConsumeInputVector();

	float TraceDepth = 1.0f;
	float SphereRadius = 50.0f;
	// float LineTraceLength = SphereRadius + TraceDepth;
	FVector StartPoint = UpdatedComponent->GetComponentLocation();
	FVector EndPoint = StartPoint - TraceDepth * FVector::UpVector;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bWasFalling = bFalling;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	bFalling = !GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_Visibility, Sphere, CollisionParams);
	if (bFalling)
	{
		VerticalVelocity += GetGravityZ() * FVector::UpVector * DeltaTime;
	}
	else if (bWasFalling && VerticalVelocity.Z < 0.0f)
	{
		VerticalVelocity = FVector::ZeroVector;
		bJump = false;
	}

	Velocity += VerticalVelocity;
	FVector Delta = Velocity * DeltaTime;
	if (!Delta.IsNearlyZero())
	{
		FQuat Rot = UpdatedComponent->GetComponentQuat();
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rot, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			// Try to slide the remaining distance along the surface.
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
	}

	UpdateComponentVelocity();
}
