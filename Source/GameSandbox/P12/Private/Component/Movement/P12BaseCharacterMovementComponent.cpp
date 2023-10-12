// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Movement/P12BaseCharacterMovementComponent.h"

#include "Curves/CurveVector.h"

UP12BaseCharacterMovementComponent::UP12BaseCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12BaseCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12BaseCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12BaseCharacterMovementComponent::DefaultSetup()
{
	NavAgentProps.bCanCrouch = true;
	SetPawnRotationMode(true);
}

void UP12BaseCharacterMovementComponent::SetPawnRotationMode(const bool bOrientToMovement)
{
	if (bOrientRotationToMovement == bOrientToMovement)
	{
		return;
	}
	bUseControllerDesiredRotation = !bOrientToMovement;
	bOrientRotationToMovement = bOrientToMovement;
}

void UP12BaseCharacterMovementComponent::ToggleMaxSpeed(const bool bRun)
{
	MaxWalkSpeed = bRun ? MaxSpeed.Run : MaxSpeed.Walk;
}

void UP12BaseCharacterMovementComponent::StartMantle(const FP12MantleMovementParams& MantleParams)
{
	CurrentMantleParams = MantleParams;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling));
	/* ::OnMovementModeChanged(...) callback. */
	/* ::PhysCustom(...) tick update.         */
}

void UP12BaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}

bool UP12BaseCharacterMovementComponent::IsMantling()
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling);
}

void UP12BaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &ThisClass::EndMantle, CurrentMantleParams.Duration);
	}
}

void UP12BaseCharacterMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)
{
	if (CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantleParams.StartTime;
		const FVector MantleCurveValue = CurrentMantleParams.Curve->GetVectorValue(ElapsedTime);
		const float PositionAlpha = MantleCurveValue.X;

		const FVector NewLocation = FMath::Lerp(CurrentMantleParams.InitLocation, CurrentMantleParams.TargetLocation, PositionAlpha);
		const FRotator NewRotation = FMath::Lerp(CurrentMantleParams.InitRotation, CurrentMantleParams.TargetRotation, PositionAlpha);

		const FVector Delta = NewLocation - GetActorLocation();
		FHitResult MantlingHitResult;
		SafeMoveUpdatedComponent(Delta, NewRotation, false, MantlingHitResult);
	}
	Super::PhysCustom(DeltaTime, Iterations);
}
