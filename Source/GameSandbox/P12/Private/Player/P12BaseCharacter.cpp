// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP12BaseCharacter::AP12BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

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
		return;
	}
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
