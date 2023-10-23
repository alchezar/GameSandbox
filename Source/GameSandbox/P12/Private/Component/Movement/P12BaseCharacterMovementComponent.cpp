// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Movement/P12BaseCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "P12/Public/Actor/Interactive/Environment/P12Ladder.h"
#include "GameFramework/Character.h"
#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"
#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Util/P12Library.h"

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
		Result = MaxSpeed.Run;
	}
	else if (bOutOfStamina)
	{
		Result = MaxSpeed.OutOfStaminaRun;
	}
	else if (IsOnLadder())
	{
		Result = MaxSpeed.LadderClimbing;
	}
	else if (GetBaseCharacterOwner()->GetIsAiming())
	{
		Result = GetBaseCharacterOwner()->GetEquipmentComponent()->GetCurrentEquippedWeapon()->GetAimSpeed();
	}
	return Result;
}

void UP12BaseCharacterMovementComponent::DefaultSetup()
{
	NavAgentProps.bCanCrouch = true;
	SetRotationMode(true);
}

void UP12BaseCharacterMovementComponent::SetRotationMode(const bool bOrientToMovement)
{
	if (bOrientRotationToMovement == bOrientToMovement)
	{
		return;
	}
	bUseControllerDesiredRotation = !bOrientToMovement;
	bOrientRotationToMovement = bOrientToMovement;
	if (GetCharacterOwner())
	{
		GetCharacterOwner()->bUseControllerRotationYaw = !bOrientToMovement;
	}
}

void UP12BaseCharacterMovementComponent::ToggleMaxSpeed(const bool bRun)
{
	MaxWalkSpeed = bRun ? MaxSpeed.Run : MaxSpeed.Walk;
}

void UP12BaseCharacterMovementComponent::StartMantle(const FP12MantleMovementParams& MantleParams)
{
	if (bLadder)
	{
		return;
	}

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

	const float ActorToLadderProjection = GetActorToLadderProjection(GetActorLocation());
	FVector TargetLocation = CurrentLadder->GetActorLocation() + FVector(-LadderToCharacterOffset, 0.f, ActorToLadderProjection);

	FRotator TargetRotation = CurrentLadder->GetActorRightVector().ToOrientationRotator();
	TargetRotation.Yaw += 180.f;

	if (CurrentLadder->GetIsOnTop())
	{
		TargetLocation = CurrentLadder->GetAttachFromTopStartLocation();
	}

	GetOwner()->SetActorLocation(TargetLocation);
	GetOwner()->SetActorRotation(TargetRotation);

	SetMovementMode(MOVE_Custom, static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder));
	/* ::OnMovementModeChanged(...) -> {if (...CustomMovementMode == CMOVE_Ladder)} callback . */
	/* ::PhysCustom(...) -> PhysLadder(...) tick update. */
}

void UP12BaseCharacterMovementComponent::DetachFromLadder(const EP12DetachFromLadderMethod DetachFromLadderMethod)
{
	bLadder = false;
	if (DetachFromLadderMethod == EP12DetachFromLadderMethod::JumpOff)
	{
		const FVector JumpDirection = CurrentLadder->GetActorRightVector() + FVector::UpVector;
		const FVector JumpVelocity = JumpDirection * JumpOffFromLadderSpeed;

		SwitchForceRotation(true, JumpDirection.ToOrientationRotator());
		SetMovementMode(MOVE_Falling);
		SetRotationMode(true);
		Launch(JumpVelocity);
		return;
	}
	if (DetachFromLadderMethod == EP12DetachFromLadderMethod::ReachingTheBottom)
	{
		SetMovementMode(MOVE_Walking);
		return;
	}
	if (DetachFromLadderMethod == EP12DetachFromLadderMethod::ReachingTheTop)
	{
		GetBaseCharacterOwner()->MantleInput(true);
		return;
	}
	SetMovementMode(MOVE_Falling);
	/* ::OnMovementModeChanged(...) -> {if (...PreviousCustomMode == CMOVE_Ladder)} callback . */
}

bool UP12BaseCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder);
}

float UP12BaseCharacterMovementComponent::GetLadderSpeedRatio() const
{
	if (!CurrentLadder)
	{
		return 0.f;
	}
	const FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return LadderUpVector.Dot(Velocity) / MaxSpeed.LadderClimbing;
}

void UP12BaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	/* Mantle */
	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &ThisClass::EndMantle, CurrentMantleParams.Duration);
		bMantle = true;
		
	}
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Mantling))
	{
		bMantle = false;
	}

	/* Ladder */
	if (MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		bLadder = true;
	}
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(EP12CustomMovementMode::CMOVE_Ladder))
	{
		CurrentLadder = nullptr;
		bLadder = false;
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

	if (HasAnimRootMotion())
	{
		FHitResult LadderHitResult;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, LadderHitResult);
		return;
	}

	const FVector NewLocation = GetActorLocation() + Delta;
	const float NewLocationProjection = GetActorToLadderProjection(NewLocation);
	const float MinPosition = GetCharacterOwner()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float MaxPosition = CurrentLadder->GetLadderHeight() - MinPosition / 2.f;
	if (NewLocationProjection < MinPosition)
	{
		// bLadder = false;
		DetachFromLadder(EP12DetachFromLadderMethod::ReachingTheBottom);
		return;
	}
	if (NewLocationProjection > MaxPosition)
	{
		// bLadder = false;
		DetachFromLadder(EP12DetachFromLadderMethod::ReachingTheTop);
		return;
	}

	FHitResult LadderHitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, LadderHitResult);
}

float UP12BaseCharacterMovementComponent::GetActorToLadderProjection(const FVector& Location) const
{
	check(CurrentLadder)

	const FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	const FVector LadderToCharVector = Location - CurrentLadder->GetActorLocation();

	return FVector::DotProduct(LadderUpVector, LadderToCharVector);
}

void UP12BaseCharacterMovementComponent::PhysicsRotation(const float DeltaTime)
{
	if (bForceRotation)
	{
		const FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
		const FRotator DeltaRot = GetDeltaRotation(DeltaTime);
		FRotator TargetRotation = ForceTargetRotation;

		/* Set the new rotation. */
		UP12Library::FixedTurn(TargetRotation, CurrentRotation, DeltaRot);
		FHitResult RotateHit;
		SafeMoveUpdatedComponent(FVector::ZeroVector, TargetRotation, false, RotateHit);
		
		if (CurrentRotation.Equals(TargetRotation))
		{
			SwitchForceRotation(false);
			return;
		}
	}
	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}

AP12BaseCharacter* UP12BaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<AP12BaseCharacter*>(CharacterOwner);
}

void UP12BaseCharacterMovementComponent::SwitchForceRotation(const bool bEnable, const FRotator& TargetRotation)
{
	bForceRotation = bEnable;
	ForceTargetRotation = TargetRotation;
	/* ::PhysicsRotation(...) tick update. */
}
