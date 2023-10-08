// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Component/P12BaseCharacterMovementComponent.h"

AP12BaseCharacter::AP12BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UP12BaseCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	
	BaseCharacterMovement = StaticCast<UP12BaseCharacterMovementComponent*>(GetCharacterMovement());
	BaseCharacterMovement->DefaultSetup();
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->TargetArmLength = CameraArmLength.Walk;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(CameraBoom);
}

void AP12BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AP12BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP12BaseCharacter::MoveInput(const FInputActionValue& Value) 
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	if (!Controller || FMath::IsNearlyZero(MoveVector.Size()))
	{
		BaseCharacterMovement->SetPawnRotationMode(true);
		return;
	}
	BaseCharacterMovement->SetPawnRotationMode(false);
	const FRotator ControllerYawRotator = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	// const FVector ForwardDirection = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::X);
	// const FVector RightDirection   = FRotationMatrix(ControllerYawRotator).GetUnitAxis(EAxis::Y);
	const FVector ForwardDirection = ControllerYawRotator.RotateVector(FVector::ForwardVector);
	const FVector RightDirection   = ControllerYawRotator.RotateVector(FVector::RightVector);
	
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
	Super::Jump();
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
	if (bRunRequest && !bRunning && !CanRun())
	{
		return;
	}
	bRunning = bRunRequest;
	BaseCharacterMovement->ToggleMaxSpeed(bRunRequest);

	ChangeCameraArmLength(bRunRequest, CameraArmLength.Run);
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
	return bCrouch || Super::CanJumpInternal_Implementation();
}

void AP12BaseCharacter::OnJumped_Implementation()
{
	if (bCrouch)
	{
		bCrouch = false;
		UnCrouch();
	}
}

bool AP12BaseCharacter::CanRun() const
{
	return true;
}

UP12BaseCharacterMovementComponent* AP12BaseCharacter::GetBaseCharacterMovement() const
{
	return BaseCharacterMovement.Get(); 
}
