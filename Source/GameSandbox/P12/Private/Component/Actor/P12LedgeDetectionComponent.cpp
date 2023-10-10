// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12LedgeDetectionComponent.h"

#include "GameSandbox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "P12/Public/Util/P12Library.h"

UP12LedgeDetectionComponent::UP12LedgeDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12LedgeDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<ACharacter>())
	CachedOwnerCharacter = StaticCast<ACharacter*>(GetOwner());
}

void UP12LedgeDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UP12LedgeDetectionComponent::DetectLedge(FP12LedgeDescription& LedgeDescription)
{
	const UCapsuleComponent* CapsuleComponent = CachedOwnerCharacter->GetCapsuleComponent();
	if (!CapsuleComponent)
	{
		return false;
	}
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CachedOwnerCharacter.Get());
	QueryParams.bTraceComplex = true;

	/* Forward check. */
	constexpr float BottomOffsetZ = 2.f;
	const FVector CharacterBottomOffset = (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomOffsetZ) * FVector::UpVector;
	const FVector CharacterBottomLocation = CachedOwnerCharacter->GetActorLocation() - CharacterBottomOffset;
	
	const float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	const float ForwardCheckCapsuleHalfHeight = (MaxLedgeHeight - MinLedgeHeight) / 2.f;

	const FVector ForwardStartLocation = CharacterBottomLocation + (MinLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	const FVector ForwardEndLocation = ForwardStartLocation + ForwardCheckDistance * CachedOwnerCharacter->GetActorForwardVector();
	
	FHitResult ForwardHitResult;
	FCollisionShape ForwardCapsule = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	GetWorld()->SweepSingleByChannel(ForwardHitResult, ForwardStartLocation, ForwardEndLocation,FQuat::Identity, ECC_CLIMBING, ForwardCapsule, QueryParams);
	DebugCapsuleTrace(ForwardHitResult, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FColor::Green);
	if (!ForwardHitResult.bBlockingHit)
	{
		return false;
	}
	
	/* Downward check. */
	constexpr float DownwardCheckDepthOffset = 10.f;
	const float DownwardCheckSphereRadius = ForwardCheckCapsuleRadius;
	
	FVector DownwardStart = ForwardHitResult.ImpactPoint - ForwardHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStart.Z = CharacterBottomLocation.Z + MaxLedgeHeight + DownwardCheckSphereRadius;
	const FVector DownwardEnd = FVector(DownwardStart.X, DownwardStart.Y, CharacterBottomLocation.Z);
	
	FHitResult DownwardHitResult;
	FCollisionShape DownwardSphere = FCollisionShape::MakeSphere(DownwardCheckSphereRadius);
	GetWorld()->SweepSingleByChannel(DownwardHitResult, DownwardStart, DownwardEnd,	FQuat::Identity, ECC_CLIMBING, DownwardSphere, QueryParams);
	DebugCapsuleTrace(DownwardHitResult, DownwardCheckSphereRadius, 0.f, FColor::Blue);
	if (!DownwardHitResult.bBlockingHit)
	{
		return false;
	}

	/* Overlap check. */
	const float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	const float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	const FVector OverlapLocation = DownwardHitResult.Location + FVector(0.f, 0.f, OverlapCapsuleHalfHeight - OverlapCapsuleRadius + BottomOffsetZ);
	const FName CollisionProfileName = CapsuleComponent->GetCollisionProfileName();
	
	FCollisionShape OverlapCapsule = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);
	if (GetWorld()->OverlapAnyTestByProfile(OverlapLocation, FQuat::Identity, CollisionProfileName, OverlapCapsule, QueryParams))
	{
		return false;
	}

	LedgeDescription.Location = DownwardHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();

	if (UP12Library::GetDrawDebugAllowed())
	{
		DrawDebugCapsule(GetWorld(), OverlapLocation, OverlapCapsuleHalfHeight, OverlapCapsuleRadius, FQuat::Identity, FColor::Red, false, 5.f, 0, 1.f);
		DrawDebugDirectionalArrow(GetWorld(), OverlapLocation, OverlapLocation + LedgeDescription.Rotation.Vector() * 50.f, 50.f, FColor::Red, false, 5.f, 0, 1.f);
	}
	return true;
}

void UP12LedgeDetectionComponent::DebugCapsuleTrace(const FHitResult& HitResult, const float Radius, const float HalfHeight, const FColor Color)
{
	if (!UP12Library::GetDrawDebugAllowed())
	{
		return;
	}
	
	constexpr float DrawTime = 5.f;
	constexpr float SuccessThickness = 1.f;
	
	if (HitResult.bBlockingHit)
	{
		DrawDebugCapsule(GetWorld(), HitResult.Location, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime, 0, SuccessThickness);
		DrawDebugDirectionalArrow(GetWorld(), HitResult.TraceStart, HitResult.Location, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	else
	{
		DrawDebugCapsule(GetWorld(), HitResult.TraceEnd, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
		DrawDebugDirectionalArrow(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	DrawDebugCapsule(GetWorld(), HitResult.TraceStart, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
}
