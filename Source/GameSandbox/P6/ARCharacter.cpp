// Copyright (C) 2023, IKinder

#include "ARCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddMappingContext();
}

void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARCharacter::AddMappingContext()
{
	const auto PlayerController = Cast<APlayerController>(Controller); 
	if (PlayerController) return;

	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
}

void AARCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	if (!Controller) return;
	
	AddMovementInput(GetActorRightVector(), MovementVector.X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
}

void AARCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	if (!Controller) return;
	
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

