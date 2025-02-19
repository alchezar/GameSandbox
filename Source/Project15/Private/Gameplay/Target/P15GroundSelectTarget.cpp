// Copyright © 2025, Ivan Kinder

#include "Gameplay/Target/P15GroundSelectTarget.h"

#include "Project15.h"
#include "Abilities/GameplayAbility.h"
#include "Components/DecalComponent.h"

AP15GroundSelectTarget::AP15GroundSelectTarget()
{
	PrimaryActorTick.bCanEverTick = true;
	bDestroyOnConfirmation        = false;

	Root = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(Root.Get());

	Decal = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	Decal->SetupAttachment(Root.Get());
	Decal->DecalSize = FVector(Radius);
	Decal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	Decal->SetRelativeScale3D(FVector(0.1f, 1.f, 1.f));
}

void AP15GroundSelectTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AP15GroundSelectTarget::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	Root->SetWorldLocation(GetPlayerLookingPoint());
}

void AP15GroundSelectTarget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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

	// Add actor array as the first target data (even if there are no overlapped pawns).
	FGameplayAbilityTargetData_ActorArray* ActorArrayData = new FGameplayAbilityTargetData_ActorArray{};
	ActorArrayData->TargetActorArray                      = OverlappedPawns;
	TargetData.Add(ActorArrayData);

	// Add the decal location as the second target data.
	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo{};
	CenterLocation->TargetLocation.LiteralTransform         = Decal ? Decal->GetComponentTransform() : FTransform::Identity;
	CenterLocation->TargetLocation.LocationType             = EGameplayAbilityTargetingLocationType::LiteralTransform;
	TargetData.Add(CenterLocation);

	TargetDataReadyDelegate.Broadcast(MoveTemp(TargetData));
}

FVector AP15GroundSelectTarget::GetPlayerLookingPoint(bool* Result) const
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	PrimaryPC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FHitResult            HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PrimaryPC->GetPawn());
	constexpr float Distance = 10000.f;
	const FVector   End      = ViewLocation + ViewRotation.Vector() * Distance;

	const bool bTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, End, ECC_Camera, Params);
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
