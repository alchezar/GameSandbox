// Copyright (C) 2023. IKinder

#include "GameSandbox/P1/Character/TrooperCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameSandbox/P1/PickUp/PickupBase.h"

#define OUT

#pragma region Default

ATrooperCharacter::ATrooperCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupComponents();
}

void ATrooperCharacter::BeginPlay()
{
	Super::BeginPlay();
	MappingContext();
}

void ATrooperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion // Default

#pragma region Input

void ATrooperCharacter::MappingContext() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATrooperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInput)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &Super::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		}
	}
}

void ATrooperCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		FRotator      Rotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);
		const FVector Forward  = Rotation.Vector();
		AddMovementInput(Forward, MovementVector.Y);

		Rotation.Roll       = GetControlRotation().Roll;
		const FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void ATrooperCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

#pragma endregion // Input

#pragma region Component

void ATrooperCharacter::SetupComponents()
{
	Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	Spring->SetupAttachment(RootComponent);
	Spring->TargetArmLength         = 300.f;
	Spring->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Spring);

	GetCharacterMovement()->bOrientRotationToMovement     = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

USpringArmComponent* ATrooperCharacter::GetCameraSpring() const
{
	return Spring;
}

UCameraComponent* ATrooperCharacter::GetCamera() const
{
	return Camera;
}

#pragma endregion // Component
