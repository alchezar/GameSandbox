// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13LegAlignmentComponent.h"

#include "GameFramework/Character.h"

UP13LegAlignmentComponent::UP13LegAlignmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13LegAlignmentComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = GetOwner<ACharacter>();
}

void UP13LegAlignmentComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LegsIKFloorAlignment();
}

void UP13LegAlignmentComponent::InitLegAlignment(const FName& InLeftVirtualBoneName, const FName& InRightVirtualBoneName, const bool bStart)
{
	LeftVirtualBoneName = InLeftVirtualBoneName;
	RightVirtualBoneName = InRightVirtualBoneName;
	
	LegAlignment(bStart);
}

void UP13LegAlignmentComponent::LegAlignment(const bool bEnable)
{
	SetComponentTickEnabled(bEnable);

	if (!bEnable)
	{
		IKLeftLegOffset = 0.f;
		IKRightLegOffset = 0.f;
		IKHitOffset = 0.f;
	}
}

float UP13LegAlignmentComponent::GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance, const float FromBoneToBottom) const
{
	const FVector SocketLocation = CachedCharacter->GetMesh()->GetBoneLocation(VirtualBoneName);
	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceHalfDistance);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceHalfDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CachedCharacter.Get());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (HitResult.bBlockingHit)
	{
		return (SocketLocation - HitResult.ImpactPoint).Z - FromBoneToBottom;
	}
	return 0.f;
}

void UP13LegAlignmentComponent::LegsIKFloorAlignment()
{
	constexpr float MinDistanceThreshold = 12.f;
	constexpr float IKOffsetInterp = 10.f;

	float LeftOffset = GetIKSocketOffset(LeftVirtualBoneName);
	float RightOffset = GetIKSocketOffset(RightVirtualBoneName);
	float HipOffset = 0.f;

	if (FMath::Abs(LeftOffset) > MinDistanceThreshold || FMath::Abs(RightOffset) > MinDistanceThreshold)
	{
		if (LeftOffset > RightOffset)
		{
			HipOffset = LeftOffset;
			LeftOffset = 0.f;
			RightOffset -= HipOffset;
		}
		else
		{
			HipOffset = RightOffset;
			RightOffset = 0.f;
			LeftOffset -= HipOffset;
		}
	}

	IKLeftLegOffset = FMath::FInterpTo(IKLeftLegOffset, LeftOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKRightLegOffset = FMath::FInterpTo(IKRightLegOffset, RightOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKHitOffset = FMath::FInterpTo(GetIKHipOffset(), HipOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
}
