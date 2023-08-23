// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Weapon/P10Projectile.h"

AP10Character::AP10Character()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("GunMeshSkeletalComponent");
	GunMeshComponent->SetupAttachment(GetMesh(), HandSocketName);

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("PawnNoiseEmitterComponent");

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AP10Character::BeginPlay()
{
	Super::BeginPlay();
	check(ProjectileClass)
	check(FireSound)
	check(FireEffect)

	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP10Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocallyControlled())
	{
		/* If there is no AimOffset in AnimInstance, replace mesh rotation pitch with replicated decompressed RemoteViewPitch */
		FRotator NewRot = ArmComponent->GetRelativeRotation();
		NewRot.Pitch = RemoteViewPitch * 360.f / 256.f;
		ArmComponent->SetRelativeRotation(NewRot);
	}
}

void AP10Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bCarryingObjective);
	// DOREPLIFETIME_CONDITION(ThisClass, bCarryingObjective, COND_OwnerOnly)
}

void AP10Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::FireInput, true);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::FireInput, false);
	
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimInput, true);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimInput, false);
	
}

void AP10Character::MoveInput(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AP10Character::LookInput(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D LookingVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookingVector.X);
	AddControllerPitchInput(LookingVector.Y);
}

void AP10Character::FireInput(const bool bShoot)
{
	bShooting = bShoot;
	if (!bShoot) return;

	Server_Fire();
	
	/* Play sound and effect. */
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GunMeshComponent->GetComponentLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffect, GunMeshComponent, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void AP10Character::Server_Fire_Implementation()
{
	/* Spawn Projectile actor. */
	const FVector MuzzleLocation = GunMeshComponent->GetSocketLocation(MuzzleSocketName);
	const FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation(MuzzleSocketName);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	Params.Instigator = this;
	GetWorld()->SpawnActor<AP10Projectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
}

bool AP10Character::Server_Fire_Validate()
{
	return true;
}

void AP10Character::AimInput(const bool bAim)
{
	bAiming = bAim;
}
