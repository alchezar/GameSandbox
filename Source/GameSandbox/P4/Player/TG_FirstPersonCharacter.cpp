// Copyright (C) 2023, IKinder

#include "../Player/TG_FirstPersonCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../Weapon/TG_Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

ATG_FirstPersonCharacter::ATG_FirstPersonCharacter()
{
	// bHasRifle = false;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FP_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FP_CameraComponent->SetupAttachment(GetMesh(), HeadSocketName);
	FP_CameraComponent->bUsePawnControlRotation = true;
}

void ATG_FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}	
	if (WeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ATG_Gun>(WeaponClass);
		if (!CurrentWeapon) return;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), HandSocketName);
		CurrentWeapon->SetActorRelativeLocation(FVector(-2.f, -12.f, -1.f));
		CurrentWeapon->SetActorRelativeRotation(FRotator(-20.f, 0.f, 0.f));
		CurrentWeapon->SetAnimInstance(GetMesh()->GetAnimInstance());

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, CurrentWeapon, &ATG_Gun::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, CurrentWeapon, &ATG_Gun::StopFire);
	}
	if (FP_CameraComponent)
	{
		FP_CameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HeadSocketName);
		FP_CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); 
	}
}

void ATG_FirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// check(CurrentWeapon);

	/* Set up action bindings */
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	/* Jumping */
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	/* Moving */
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	/* Looking */
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	// /* Firing */
	// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, CurrentWeapon, &ATG_Gun::StartFire);
	// EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, CurrentWeapon, &ATG_Gun::StopFire);
}

void ATG_FirstPersonCharacter::Move(const FInputActionValue& Value)
{
	/* input is a Vector2D */
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller) return;

	/* add movement */
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ATG_FirstPersonCharacter::Look(const FInputActionValue& Value)
{
	/* input is a Vector2D */
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!Controller) return;

	/* add yaw and pitch input to controller */
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATG_FirstPersonCharacter::SetHasRifle(const bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATG_FirstPersonCharacter::GetHasRifle() const
{
	return bHasRifle;
}

UCameraComponent* ATG_FirstPersonCharacter::GetFirstPersonCameraComponent() const
{
	return FP_CameraComponent;
}

ATG_Gun* ATG_FirstPersonCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}
