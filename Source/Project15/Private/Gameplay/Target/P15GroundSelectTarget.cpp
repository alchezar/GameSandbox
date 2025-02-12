// Copyright © 2025, Ivan Kinder

#include "Gameplay/Target/P15GroundSelectTarget.h"

#include "Project15.h"
#include "Abilities/GameplayAbility.h"

AP15GroundSelectTarget::AP15GroundSelectTarget()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP15GroundSelectTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AP15GroundSelectTarget::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

#if 1
	DrawDebugCapsule(GetWorld(), GetPlayerLookingPoint(), Radius, Radius, FQuat::Identity, FColor::Orange);
#endif
}

void AP15GroundSelectTarget::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	PrimaryPC = Ability->GetOwningActorFromActorInfo()->GetInstigatorController<APlayerController>();
}

void AP15GroundSelectTarget::ConfirmTargetingAndContinue()
{
	const TArray<TWeakObjectPtr<AActor>> OverlappedPawns = GetOverlappedPawns(GetPlayerLookingPoint());
	FGameplayAbilityTargetDataHandle     TargetData      = {};

	if (!OverlappedPawns.IsEmpty())
	{
		TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedPawns);
	}

	TargetDataReadyDelegate.Broadcast(MoveTemp(TargetData));
}

FVector AP15GroundSelectTarget::GetPlayerLookingPoint(bool* Result) const
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	PrimaryPC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FHitResult            HitResult;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(PrimaryPC->GetPawn());
	constexpr float Distance = 10000.f;
	const FVector   End      = ViewLocation + ViewRotation.Vector() * Distance;

	const bool bTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, End, ECC_Visibility, Params);
	if (Result)
	{
		*Result = bTrace;
	}
	return bTrace ? HitResult.ImpactPoint : FVector::ZeroVector;
}

TArray<TWeakObjectPtr<AActor>> AP15GroundSelectTarget::GetOverlappedPawns(const FVector& InViewLocation) const
{
	TArray<FOverlapResult>            Overlaps;
	const FCollisionObjectQueryParams ObjectQueryParams = {ECC_Pawn};
	FCollisionQueryParams             Params;
	Params.AddIgnoredActor(PrimaryPC->GetPawn());

	const bool bOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		InViewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams{ECC_Pawn},
		FCollisionShape::MakeSphere(Radius),
		Params);

	TArray<TWeakObjectPtr<AActor>> OverlappedActors = {};
	EARLY_RETURN_VALUE_IF(!bOverlap, OverlappedActors)
	Algo::Transform(Overlaps, OverlappedActors,
		[](const FOverlapResult& Result) -> TWeakObjectPtr<AActor> { return Result.GetActor(); });

	return OverlappedActors;
}
