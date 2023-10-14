// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Actor/Interactive/Environment/P12Ladder.h"
#include "P12/Public/Component/Actor/P12LedgeDetectionComponent.h"
#include "P12/Public/Component/MOvement/P12BaseCharacterMovementComponent.h"
#include "P12/Public/Util/P12Library.h"

AP12BaseCharacter::AP12BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UP12BaseCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	BaseCharacterMovement = StaticCast<UP12BaseCharacterMovementComponent*>(GetCharacterMovement());
	BaseCharacterMovement->DefaultSetup();

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->TargetArmLength = CameraArmLength.Walk;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(CameraBoom);

	LedgeDetection = CreateDefaultSubobject<UP12LedgeDetectionComponent>("LedgeDetectorComponent");
}

void AP12BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultMeshLocation = GetMesh()->GetRelativeLocation();
}

void AP12BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	LegsIKFloorAlignment();
}

void AP12BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP12BaseCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!BaseCharacterMovement->GetCanWalk())
	{
		BaseCharacterMovement->SetRotationMode(false);
		return;
	}
	
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (!Controller || FMath::IsNearlyZero(MoveVector.Size()))
	{
		BaseCharacterMovement->SetRotationMode(true);
		return;
	}
	BaseCharacterMovement->SetRotationMode(false);
	const FRotator ControllerYawRotator = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	// const FVector ForwardDirection = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::X);
	// const FVector RightDirection   = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::Y);
	const FVector ForwardDirection = ControllerYawRotator.RotateVector(FVector::ForwardVector);
	const FVector RightDirection = ControllerYawRotator.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void AP12BaseCharacter::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller || FMath::IsNearlyZero(LookVector.Size()))
	{
		return;
	}
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AP12BaseCharacter::JumpInput()
{
	if (!GetBaseCharacterMovement()->GetCanJump())
	{
		return;
	}
	
	Super::Jump();
}

void AP12BaseCharacter::MantleInput()
{
	if (!GetBaseCharacterMovement()->GetCanMantle())
	{
		return;
	}
	
	FP12LedgeDescription LedgeDescription;
	if (!LedgeDetection->DetectLedge(OUT LedgeDescription) || BaseCharacterMovement->IsMantling())
	{
		return;
	}

	const float MantleHeight = (LedgeDescription.Location - GetActorLocation()).Z;
	const FP12MantleSettings MantleSettings = GetMantleSettings(MantleHeight);

	float MinRange;
	float MaxRange;
	MantleSettings.Curve->GetTimeRange(MinRange, MaxRange);
	const float Duration = MaxRange - MinRange;

	const FVector2D Source = {MantleSettings.MinHeight, MantleSettings.MaxHeight};
	const FVector2D Target = {MantleSettings.MinHeightStartTime, MantleSettings.MaxHeightStartTime};
	const float StartTime = FMath::GetMappedRangeValueUnclamped(Source, Target, MantleHeight);

	const FVector InitAnimLocation = LedgeDescription.Location - MantleSettings.AnimationCorrectionZ * FVector::UpVector + MantleSettings.AnimationCorrectionXY * LedgeDescription.Normal;

	const FP12MantleMovementParams MantleParams = {GetActorLocation(), GetActorRotation(), LedgeDescription.Location, LedgeDescription.Rotation, Duration, StartTime, MantleSettings.Curve, InitAnimLocation};
	GetBaseCharacterMovement()->StartMantle(MantleParams);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}
	AnimInstance->Montage_Play(MantleSettings.Montage, 1.f, EMontagePlayReturnType::Duration, StartTime);
	// AnimInstance->Montage_JumpToSection("Mantle_High", MantleSettings.Montage);
}

void AP12BaseCharacter::CrouchInput()
{
	bCrouch = !bCrouch;
	bCrouch ? Crouch() : UnCrouch();

	ChangeCameraArmLength(bCrouch, CameraArmLength.Crouch);
}

void AP12BaseCharacter::RunInput(const bool bRunRequest)
{
	if (bCrouch)
	{
		bCrouch = false;
		UnCrouch();
	}
	if (bRunRequest && !bRunning && !GetCanRun())
	{
		return;
	}
	bRunning = bRunRequest;
	BaseCharacterMovement->ToggleMaxSpeed(bRunRequest);

	ChangeCameraArmLength(bRunRequest, CameraArmLength.Run);
}

void AP12BaseCharacter::LadderJumpInput()
{
	if (GetBaseCharacterMovement()->IsOnLadder())
	{
		GetBaseCharacterMovement()->DetachFromLadder();
		return;
	}
	const AP12Ladder* Ladder = GetAvailableLadder();
	if (!Ladder)
	{
		return;
	}
	GetBaseCharacterMovement()->AttachToLadder(Ladder);
}

void AP12BaseCharacter::LadderClimbInput(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	if (!GetBaseCharacterMovement()->IsOnLadder() || FMath::IsNearlyZero(AxisValue))
	{
		return;
	}
	const FVector LadderUpVector = GetBaseCharacterMovement()->GetCurrentLadder()->GetActorUpVector();
	AddMovementInput(LadderUpVector, AxisValue);
}

void AP12BaseCharacter::ChangeCameraArmLength(const bool bStart, const float NewArmLength)
{
	FTimerDelegate RunDelegate;
	const float TargetLength = bStart ? NewArmLength : CameraArmLength.Walk;
	RunDelegate.BindUObject(this, &ThisClass::SmoothlyChangeCameraArmLength, bStart, TargetLength);
	GetWorld()->GetTimerManager().SetTimer(RunTimer, RunDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP12BaseCharacter::SmoothlyChangeCameraArmLength(const bool bRunStart, const float TargetLength)
{
	constexpr float InterpSpeed = 2.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, TargetLength))
	{
		GetWorld()->GetTimerManager().ClearTimer(RunTimer);
	}
}

void AP12BaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CameraBoom->TargetOffset = FVector(0.f, 0.f, HalfHeightAdjust);
}

void AP12BaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CameraBoom->TargetOffset = FVector(0.f, 0.f, 0.f);
}

bool AP12BaseCharacter::CanJumpInternal_Implementation() const
{
	return bCrouch || !GetBaseCharacterMovement()->IsMantling() || Super::CanJumpInternal_Implementation();
}

void AP12BaseCharacter::OnJumped_Implementation()
{
	if (bCrouch)
	{
		bCrouch = false;
		UnCrouch();
	}
}

bool AP12BaseCharacter::GetCanRun() const
{
	return true;
}

UP12BaseCharacterMovementComponent* AP12BaseCharacter::GetBaseCharacterMovement() const
{
	return BaseCharacterMovement.Get();
}

float AP12BaseCharacter::GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance, const float FromBoneToBottom)
{
	const FVector SocketLocation = GetMesh()->GetBoneLocation(VirtualBoneName);
	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceHalfDistance);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceHalfDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	UP12Library::DrawDebugLineTrace(GetWorld(), HitResult, UP12Library::GetCanDrawDebugLegAlignment());
	if (HitResult.bBlockingHit)
	{
		return (SocketLocation - HitResult.ImpactPoint).Z - FromBoneToBottom;
	}
	return 0.f;
}

void AP12BaseCharacter::LegsIKFloorAlignment()
{
	constexpr float MinDistanceThreshold = 12.f;
	constexpr float IKOffsetInterp = 10.f;

	float LeftOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_l_ankle");
	float RightOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_r_ankle");
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

const FP12MantleSettings& AP12BaseCharacter::GetMantleSettings(const float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

void AP12BaseCharacter::RegisterInteractiveActor(AP12InteractiveActor* NewInteractiveActor)
{
	if (AvailableInteractiveActors.Contains(NewInteractiveActor))
	{
		return;
	}
	AvailableInteractiveActors.Add(NewInteractiveActor);
}

void AP12BaseCharacter::UnregisterInteractiveActor(AP12InteractiveActor* OldInteractiveActor)
{
	if (!AvailableInteractiveActors.Contains(OldInteractiveActor))
	{
		return;
	}
	AvailableInteractiveActors.Remove(OldInteractiveActor);
}

const AP12Ladder* AP12BaseCharacter::GetAvailableLadder() const
{
	const AP12Ladder* Result = nullptr;
	for (const AP12InteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (!InteractiveActor->IsA<AP12Ladder>())
		{
			continue;
		}
		Result = StaticCast<const AP12Ladder*>(InteractiveActor);
		break;
	}
	return Result;
}
