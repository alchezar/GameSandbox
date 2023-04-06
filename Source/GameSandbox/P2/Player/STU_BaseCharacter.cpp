// Copyright (C) 2023, IKinder

#include "STU_BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASTU_BaseCharacter::ASTU_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupComponent();
}

void ASTU_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	MappingContext();
}

void ASTU_BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("%f"), DefaultMaxWalkSpeed));
}

#pragma region Component

void ASTU_BaseCharacter::SetupComponent()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

UCameraComponent* ASTU_BaseCharacter::GetCameraComp() const
{
	return CameraComponent;
}

#pragma endregion // Component

#pragma region Input

void ASTU_BaseCharacter::MappingContext() const
{
	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ASTU_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!PlayerInputComponent) return;

	auto* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;

	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &Super::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &ThisClass::StartRun);
	EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::StopRun);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchToggle);
}

void ASTU_BaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller) return;

	FRotator      Rotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);
	const FVector Forward  = Rotation.Vector();
	AddMovementInput(Forward, MovementVector.Y);

	Rotation.Roll       = GetControlRotation().Roll;
	const FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(Right, MovementVector.X);
}

void ASTU_BaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ASTU_BaseCharacter::StartRun()
{
	bRunning                             = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
}

void ASTU_BaseCharacter::StopRun()
{
	bRunning                             = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

bool ASTU_BaseCharacter::GetIsRunning() const
{
	return bRunning;
}

void ASTU_BaseCharacter::CrouchToggle()
{
	GetCharacterMovement()->IsCrouching() ? UnCrouch() : Crouch();
}

#pragma endregion // Input
