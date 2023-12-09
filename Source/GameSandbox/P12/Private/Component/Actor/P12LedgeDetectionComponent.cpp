// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12LedgeDetectionComponent.h"

#include "GameSandbox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "P12/Public/Util/P12CoreTypes.h"
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
	GetWorld()->SweepSingleByChannel(ForwardHitResult, ForwardStartLocation, ForwardEndLocation, FQuat::Identity, ECC_CLIMBING, ForwardCapsule, QueryParams);
	UP12Library::DrawDebugCapsuleTrace(GetWorld(), ForwardHitResult, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FColor::Green, UP12Library::GetCanDrawDebugLedgeDetection());
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
	GetWorld()->SweepSingleByChannel(DownwardHitResult, DownwardStart, DownwardEnd, FQuat::Identity, ECC_CLIMBING, DownwardSphere, QueryParams);
	UP12Library::DrawDebugCapsuleTrace(GetWorld(), DownwardHitResult, DownwardCheckSphereRadius, 0.f, FColor::Blue, UP12Library::GetCanDrawDebugLedgeDetection());
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
	if (GetWorld()->OverlapBlockingTestByProfile(OverlapLocation, FQuat::Identity, CollisionProfileName, OverlapCapsule, QueryParams))
	{
		return false;
	}

	LedgeDescription.Location = OverlapLocation;
	LedgeDescription.Rotation = (ForwardHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();
	LedgeDescription.Normal = ForwardHitResult.ImpactNormal;
	UP12Library::DrawDebugDirectionalCapsule(GetWorld(), LedgeDescription, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, OverlapLocation, UP12Library::GetCanDrawDebugLedgeDetection());
	return true;
}
