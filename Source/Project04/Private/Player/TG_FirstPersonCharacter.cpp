// Copyright (C) 2023, IKinder

#include "Player/TG_FirstPersonCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Weapon/TG_Gun.h"

ATG_FirstPersonCharacter::ATG_FirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// bHasRifle = false;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FP_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FP_CameraComponent->SetupAttachment(GetMesh(), HeadSocketName);
	FP_CameraComponent->bUsePawnControlRotation = true;

	EffectVolume = CreateDefaultSubobject<UBoxComponent>("EffectVolume");
	EffectVolume->SetupAttachment(GetRootComponent());
	EffectVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	EffectVolume->SetCollisionProfileName("NoCollision");
	DamagePostProcess = CreateDefaultSubobject<UPostProcessComponent>("DamagePostProcess");
	DamagePostProcess->SetupAttachment(EffectVolume);
	DamagePostProcess->bUnbound = false;
	DamagePostProcess->BlendWeight = 0.f;

	DeathCameraPosition = CreateDefaultSubobject<UArrowComponent>("DeathCameraPosition");
	DeathCameraPosition->SetupAttachment(GetRootComponent());
}

void ATG_FirstPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, CurrentWeapon, &ATG_Gun::PullTrigger);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, CurrentWeapon, &ATG_Gun::ReleaseTrigger);
	}

	if (FP_CameraComponent)
	{
		FP_CameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HeadSocketName);
		FP_CameraComponent->SetRelativeLocation(CameraOffset);
	}
	MaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed();
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
	/* Firing */
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, CurrentWeapon, &ATG_Gun::PullTrigger);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, CurrentWeapon, &ATG_Gun::ReleaseTrigger);
	/* Aiming */
	EnhancedInputComponent->BindAction<Super, bool>(AimAction, ETriggerEvent::Started, this, &Super::SetIsAiming, true);
	EnhancedInputComponent->BindAction<Super, bool>(AimAction, ETriggerEvent::Completed, this, &Super::SetIsAiming, false);
	/* Running */
	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Started, this, &ThisClass::Run, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Completed, this, &ThisClass::Run, false);
}

void ATG_FirstPersonCharacter::CharacterDying()
{
	Super::CharacterDying();
	
	AController* OldController = GetController();
	GetController()->UnPossess();
	
	APlayerController* PlayerController = Cast<APlayerController>(OldController);
	if (!PlayerController) return;

	if (Cast<ACameraActor>(PlayerController->GetViewTarget())) return;

	const FActorSpawnParameters SpawnParameters;
	const FTransform Transform = DeathCameraPosition->GetComponentTransform();	
	ACameraActor* DeathCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform, SpawnParameters);
	if (!DeathCamera) return;
	
	PlayerController->SetViewTargetWithBlend(DeathCamera, 1.f);
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

void ATG_FirstPersonCharacter::Run(const bool bRun)
{
	GetCharacterMovement()->MaxWalkSpeed = bRun ? MaxWalkSpeed * 2 : MaxWalkSpeed;
}

void ATG_FirstPersonCharacter::SetHasRifle(const bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

void ATG_FirstPersonCharacter::ReceiveDamage(const float Damage)
{
	Super::ReceiveDamage(Damage);
	OnHealthChanged.Broadcast(GetCurrentHealth());
	ApplyDamageEffect();
}

void ATG_FirstPersonCharacter::SetHealth(const float NewHealth)
{
	Super::SetHealth(NewHealth);
	OnHealthChanged.Broadcast(GetCurrentHealth());
}

float ATG_FirstPersonCharacter::GetCurrentHealth() const
{
	return Health;
}

bool ATG_FirstPersonCharacter::GetHasRifle() const
{
	return bHasRifle;
}

UCameraComponent* ATG_FirstPersonCharacter::GetFirstPersonCameraComponent() const
{
	return FP_CameraComponent;
}

void ATG_FirstPersonCharacter::PossessedBy(AController* NewController)
{
	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (!PlayerController) return;

	PlayerController->SetViewTargetWithBlend(this);
	// Super::PossessedBy(NewController);
}

void ATG_FirstPersonCharacter::ApplyDamageEffect()
{
	Alpha = 0.f;
	GetWorldTimerManager().SetTimer(DamageTimer, this, &ThisClass::PlayDamageEffect, GetWorld()->DeltaTimeSeconds, true);

}

void ATG_FirstPersonCharacter::PlayDamageEffect()
{
	const float Pi = 2 * acos(0.0);
	const float Delta = GetWorld()->DeltaTimeSeconds;
	
	if (Alpha > 2.f)
	{
		GetWorldTimerManager().ClearTimer(DamageTimer);
		Alpha = 0.f;
		DamagePostProcess->BlendWeight = 0.f;
		return;
	}
		
	DamagePostProcess->BlendWeight = 0.5 - cos(Alpha * Pi) / 2;
	Alpha += Delta * 5;
	
}
