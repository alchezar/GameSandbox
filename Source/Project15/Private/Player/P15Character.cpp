// Copyright © 2025, Ivan Kinder

#include "Player/P15Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP15Character::AP15Character()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmCameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent.Get());

	PlayerEye = CreateDefaultSubobject<UCameraComponent>("PlayerEyeCameraComponent");
	PlayerEye->SetupAttachment(CameraBoom.Get());
}

void AP15Character::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(InputContext, 0);
			}
		}
	}
}

void AP15Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP15Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		Input->BindAction(LookAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		Input->BindAction(JumpAction.Get(), ETriggerEvent::Started, this, &Super::Jump);
		Input->BindAction(JumpAction.Get(), ETriggerEvent::Completed | ETriggerEvent::Canceled, this, &Super::StopJumping);
	}
}

void AP15Character::MoveInput(const FInputActionValue& InputValue)
{
	if (!Controller)
	{
		return;
	}

	const FVector ForwardDirection = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::ForwardVector);
	const FVector RightDirection   = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::RightVector);

	const FVector2D InputVector = InputValue.Get<FVector2D>();
	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AP15Character::LookInput(const FInputActionValue& InputValue)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D InputVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}
