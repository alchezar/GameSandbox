// Copyright © 2024, Ivan Kinder

#include "Player/P14Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/P14InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP14Character::AP14Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = {0.f, 540.f, 0.f};
		Movement->JumpZVelocity             = 600.f;
		Movement->AirControl                = 0.2f;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength         = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UP14InventoryComponent>("InventoryComponent");
}

void AP14Character::BeginPlay()
{
	Super::BeginPlay();

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetController<APlayerController>()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}
}

void AP14Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed | ETriggerEvent::Canceled, this, &Super::StopJumping);
	}
}

void AP14Character::MoveInput(const FInputActionValue& InputValue)
{
	check(Controller)

	const FVector2D InputVector      = InputValue.Get<FVector2D>();
	const FVector   ForwardDirection = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::ForwardVector);
	const FVector   RightDirection   = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AP14Character::LookInput(const FInputActionValue& InputValue)
{
	const FVector2D InputVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}
