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

float UP12BaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bRunning)
	{
		Result = RunSpeed;
	}
	else if (bOutOfStamina)
	{
		Result =  OutOfStaminaSpeed;
	}
	else if (IsOnLadder())
	{
		Result = LadderClimbingMaxSpeed;
	}
	return Result;
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

void UP12BaseCharacterMovementComponent::Run(const bool bStart)
{
	bRunning = bStart;
	bForceMaxAccel = bStart;
}

void UP12BaseCharacterMovementComponent::StartMantle(const FP12MantleMovementParams& MantleParams)
{
	CurrentMantleParams = MantleParams;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling));
	/* ::OnMovementModeChanged(...) -> {if (...CustomMovementMode == CMOVE_Mantling)} callback. */
	/* ::PhysCustom(...) -> PhysMantling(...) tick update. */
}

void UP12BaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}

bool UP12BaseCharacterMovementComponent::IsMantling()
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling);
}

void UP12BaseCharacterMovementComponent::AttachToLadder(const AP12Ladder* Ladder) 
{
	CurrentLadder = Ladder;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder));
	/* ::OnMovementModeChanged(...) -> {if (...CustomMovementMode == CMOVE_Ladder)} callback . */
	/* ::PhysCustom(...) -> PhysLadder(...) tick update. */
}

void UP12BaseCharacterMovementComponent::DetachFromLadder() 
{
	SetMovementMode(MOVE_Falling);
	/* ::OnMovementModeChanged(...) -> {if (...PreviousCustomMode == CMOVE_Ladder)} callback . */
}

bool UP12BaseCharacterMovementComponent::IsOnLadder() const 
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder);
}


void UP12BaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &ThisClass::EndMantle, CurrentMantleParams.Duration);
	}
	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		// do ladder
	}
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		CurrentLadder = nullptr;
	}
}

void UP12BaseCharacterMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)
{
	if (CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		PhysMantling(DeltaTime, Iterations);
	}
	if (CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		PhysLadder(DeltaTime, Iterations);
	}
	Super::PhysCustom(DeltaTime, Iterations);
}

void UP12BaseCharacterMovementComponent::PhysMantling(const float DeltaTime, const int32 Iterations)
{
	const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantleParams.StartTime;
	const FVector MantleCurveValue = CurrentMantleParams.Curve->GetVectorValue(ElapsedTime);
	const float PositionAlpha = MantleCurveValue.X;
		
	const float XYCorrectionAlpha = MantleCurveValue.Y;
	const float ZCorrectionAlpha = MantleCurveValue.Z;
	FVector CorrectedInitLocation = FMath::Lerp(CurrentMantleParams.InitLocation, CurrentMantleParams.InitAnimationLocation, XYCorrectionAlpha);
	CorrectedInitLocation.Z = FMath::Lerp(CurrentMantleParams.InitLocation.Z, CurrentMantleParams.InitAnimationLocation.Z, ZCorrectionAlpha);
		
	const FVector NewLocation = FMath::Lerp(CorrectedInitLocation, CurrentMantleParams.TargetLocation, PositionAlpha);
	const FRotator NewRotation = FMath::Lerp(CurrentMantleParams.InitRotation, CurrentMantleParams.TargetRotation, PositionAlpha);

	const FVector Delta = NewLocation - GetActorLocation();
	FHitResult MantlingHitResult;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, MantlingHitResult);
}

void UP12BaseCharacterMovementComponent::PhysLadder(const float DeltaTime, const int32 Iterations)
{
	constexpr float Friction = 1.f;
	CalcVelocity(DeltaTime, Friction, false, LadderClimbingDeceleration);
	const FVector Delta = Velocity * DeltaTime;

	FHitResult LadderHitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, LadderHitResult);
}
