// Copyright (C) 2023, IKinder

#include "P8/Public/Component/P8MovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "P8/Public/Util/P8Utils.h"

UP8MovementComponent::UP8MovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP8MovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP8MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		LastMove = FP8Move(GetMoveAlpha(), GetTurnAlpha(), DeltaTime, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
		SimulateMove(LastMove);
	}
}

void UP8MovementComponent::SimulateMove(const FP8Move& TheMove)
{
	check(Mass > 0.f)
	/* Calculate Resistances */
	const double NormalForce = Mass * (-GetWorld()->GetGravityZ() / 100);
	const FVector RollingResistance = RollingCoefficient * NormalForce * Velocity.GetSafeNormal();
	const FVector AirResistance = DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
	/* Find kart Velocity */
	const FVector Force = GetOwner()->GetActorForwardVector() * MaxMoveForce * TheMove.MoveAlpha - AirResistance - RollingResistance;
	const FVector Acceleration = Force / Mass;
	Velocity += Acceleration * TheMove.DeltaTime; 
	FHitResult HitResult;
	GetOwner()->AddActorWorldOffset(Velocity * 100.0 * TheMove.DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
	{
		Velocity = FVector::ZeroVector;
	}

	const float RotationAngle = (GetOwner()->GetActorForwardVector().Dot(Velocity) * TheMove.DeltaTime) / MinTurnRadius * TheMove.TurnAlpha;
	const FQuat RotationDelta = FQuat(GetOwner()->GetActorUpVector(), RotationAngle);
	GetOwner()->AddActorWorldRotation(RotationDelta, true);
	Velocity = RotationDelta.RotateVector(Velocity);
}

void UP8MovementComponent::Move(const FVector2D Value)
{
	MoveAlpha = Value.Y;
	TurnAlpha = Value.X;
}
