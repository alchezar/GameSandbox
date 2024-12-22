// Copyright (C) 2023, IKinder

#include "LS_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P5/Weapon/LS_LightSaber.h"

ALS_PlayerCharacter::ALS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 0.f);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
}

void ALS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ALS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &Super::StartMoving);	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &Super::StopMoving);	
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &Super::Attack);

	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Started, this, &ThisClass::Run, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Completed, this, &ThisClass::Run, false);

	EnhancedInputComponent->BindAction(TakeWeaponAction, ETriggerEvent::Started, this, &Super::TakeWeapon);
}

void ALS_PlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	if (!Controller) return;

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ALS_PlayerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	TurnInPlace();
}

void ALS_PlayerCharacter::Jump()
{
	Super::Jump();
	// GetCurrentSaber()->TurnBeamOff();
}

void ALS_PlayerCharacter::StopJumping()
{
	Super::StopJumping();
}

void ALS_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCurrentSaber()->TurnBeamOn();
}

void ALS_PlayerCharacter::Run(const bool bRun)
{
	GetCharacterMovement()->MaxWalkSpeed = bRun ? MaxWalkSpeed * 2 : MaxWalkSpeed;
	// bRun ? GetCurrentSaber()->TurnBeamOff() : GetCurrentSaber()->TurnBeamOn();
}

void ALS_PlayerCharacter::TurnInPlace()
{
	const double RotationDelta = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, GetControlRotation().Yaw);
	constexpr double Tolerance = 10.0;

	if (GetVelocity().Size() > Tolerance)
	{		
		EnableTurning(true, false, 360.f);
	}
	else if (RotationDelta < -90 || RotationDelta > 90)
	{
		bTurnLeft = RotationDelta < -90;
		bTurnRight = RotationDelta > 90;
		EnableTurning(true, false, 120.f);
	}
	else if (FMath::Abs(RotationDelta) < Tolerance)
	{
		bTurnLeft = false;
		bTurnRight = false;
		EnableTurning(false, true, 120.f);
	}
}

void ALS_PlayerCharacter::EnableTurning(const bool bUseDesire, const bool bOrient, const float Rate)
{
	GetCharacterMovement()->bUseControllerDesiredRotation = bUseDesire;
	GetCharacterMovement()->bOrientRotationToMovement = bOrient;
	GetCharacterMovement()->RotationRate = FRotator(0.f, Rate, 0.f);
}

bool ALS_PlayerCharacter::GetTurnLeft() const
{
	return bTurnLeft;
}

bool ALS_PlayerCharacter::GetTurnRight() const
{
	return bTurnRight;
}