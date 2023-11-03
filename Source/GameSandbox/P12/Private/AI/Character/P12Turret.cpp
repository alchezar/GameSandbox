// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Character/P12Turret.h"

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

AP12Turret::AP12Turret()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* TurretRoot = CreateDefaultSubobject<USceneComponent>("TurretRootComponent");
	SetRootComponent(TurretRoot);
	
	TurretBaseComponent = CreateDefaultSubobject<USceneComponent>("TurretBaseSceneComponent");
	TurretBaseComponent->SetupAttachment(RootComponent);
	
	TurretBarrelComponent = CreateDefaultSubobject<USceneComponent>("TurretBarrelSceneComponent");
	TurretBarrelComponent->SetupAttachment(TurretBaseComponent);

	BarrelComponent = CreateDefaultSubobject<UP12WeaponBarrelComponent>("TurretWeaponBarrelComponent");
	BarrelComponent->SetupAttachment(TurretBarrelComponent);

}

void AP12Turret::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TurretState == EP12TurretState::Searching)
	{
		SearchingMovement(DeltaTime);
	}
	else if (TurretState == EP12TurretState::Firing)
	{
		FiringMovement(DeltaTime);
	}
}

FVector AP12Turret::GetPawnViewLocation() const
{
	// return Super::GetPawnViewLocation();
	return BarrelComponent->GetComponentLocation();
}

FRotator AP12Turret::GetViewRotation() const
{
	// return Super::GetViewRotation();
	return BarrelComponent->GetComponentRotation();	
}

void AP12Turret::SetCurrentTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	const EP12TurretState NewState = CurrentTarget ? EP12TurretState::Firing : EP12TurretState::Searching;
	SetCurrentTurretState(NewState);
}

void AP12Turret::SetCurrentTurretState(const EP12TurretState NewState)
{
	TurretState = NewState;
}

void AP12Turret::SearchingMovement(const float DeltaTime) 
{
	FRotator TurretBaseRotation = TurretBaseComponent->GetRelativeRotation();
	TurretBaseRotation.Yaw += DeltaTime * BaseSearchingRotationRate;
	TurretBaseComponent->SetRelativeRotation(TurretBaseRotation);

	FRotator TurretBarrelRotation = TurretBarrelComponent->GetRelativeRotation();
	TurretBarrelRotation.Pitch = FMath::FInterpTo(TurretBarrelRotation.Pitch, 0.f, DeltaTime, BarrelPitchRotationRate);
	TurretBarrelComponent->SetRelativeRotation(TurretBarrelRotation);
}

void AP12Turret::FiringMovement(const float DeltaTime) 
{
	const FVector LookAtDirection = (CurrentTarget->GetActorLocation() -TurretBaseComponent->GetComponentLocation()).GetSafeNormal2D();
	const FQuat LookAtQuat = LookAtDirection.ToOrientationQuat();
	const FQuat TargetQuat = FMath::QInterpTo(TurretBaseComponent->GetComponentQuat(), LookAtQuat, DeltaTime, BaseFiringInterpSpeed);
	TurretBaseComponent->SetWorldRotation(TargetQuat);

	const FVector LookDownDirection = (CurrentTarget->GetActorLocation() - TurretBarrelComponent->GetComponentLocation()).GetSafeNormal();
	const FRotator LookAtRotation = LookDownDirection.ToOrientationRotator();
	const float ClampedPitch = FMath::RInterpTo(TurretBarrelComponent->GetComponentRotation(), LookAtRotation, DeltaTime, BaseFiringInterpSpeed).Pitch;
	const FRotator ClampedRotation = FRotator(FMath::Clamp(ClampedPitch, BarrelPitchAngle.Min, BarrelPitchAngle.Max), 0.f, 0.f);
	TurretBarrelComponent->SetRelativeRotation(ClampedRotation);
}
