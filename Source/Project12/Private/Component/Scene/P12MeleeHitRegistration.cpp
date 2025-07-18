// Copyright (C) 2023, IKinder

#include "Component/Scene/P12MeleeHitRegistration.h"

#include "Project12.h"
#include "Util/P12Library.h"

UP12MeleeHitRegistration::UP12MeleeHitRegistration()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius                      = 5.f;
	BodyInstance.SetCollisionProfileName("NoCollision");
}

void UP12MeleeHitRegistration::BeginPlay()
{
	Super::BeginPlay();
}

void UP12MeleeHitRegistration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bHitRegistration)
	{
		ProcessHitRegistration();
	}
	PreviousLocation = GetComponentLocation();
}

void UP12MeleeHitRegistration::ProcessHitRegistration()
{
	const FVector   CurrentLocation = GetComponentLocation();
	FHitResult      HitResult;
	FCollisionShape SphereShape;
	SphereShape.SetSphere(GetScaledSphereRadius());
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(GetOwner()->GetOwner());
	GetWorld()->SweepSingleByChannel(HitResult, PreviousLocation, CurrentLocation, FQuat::Identity, P12::ECC_Melee, SphereShape, QueryParams);
	UP12Library::DrawDebugSphereTrace(GetWorld(), HitResult, GetScaledSphereRadius(), PreviousLocation, CurrentLocation, UP12Library::GetCanDrawDebugFire());

	if (HitResult.bBlockingHit)
	{
		const FVector Direction = (CurrentLocation - PreviousLocation).GetSafeNormal();
		OnMeleeRegistered.Broadcast(HitResult, Direction);
	}
}
