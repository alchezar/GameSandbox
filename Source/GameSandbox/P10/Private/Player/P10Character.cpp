// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Weapon/P10Projectile.h"
#include "P10/Public/Weapon/P10Weapon.h"

AP10Character::AP10Character()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("PawnNoiseEmitterComponent");

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	JumpMaxCount = 2;
}

void AP10Character::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponClass)
	check(ProjectileClass)

	SpawnWeapon();

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

	PrintMaskAsString(CharStateMask);
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
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchInput);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpInput, true);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::JumpInput, false);

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

void AP10Character::JumpInput(bool bStart)
{
	Super::Jump();
	if (static_cast<uint8>(CharStateMask & EP10CharMask::Crouch)) return;
	CharStateMask |= EP10CharMask::Jump;
}

void AP10Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CharStateMask &= ~EP10CharMask::Jump;
}

void AP10Character::CrouchInput()
{
	bCrouch = !bCrouch;
	CharStateMask ^=  EP10CharMask::Crouch; 

	bCrouch ? Crouch() : UnCrouch();
}

void AP10Character::FireInput(const bool bShoot)
{
	bShooting = bShoot;
	CharStateMask ^= EP10CharMask::Shoot; 
	if (!bShoot) return;

	// Server_Fire();
	if (Weapon)
	{
		Weapon->StartFire();
	}
}

void AP10Character::Server_Fire_Implementation()
{
	/* Spawn Projectile actor. */
	const FVector MuzzleLocation = Weapon->GetWeaponComponent()->GetSocketLocation(Weapon->GetFirstSocketName());
	const FRotator MuzzleRotation = Weapon->GetWeaponComponent()->GetSocketRotation(Weapon->GetFirstSocketName());
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
	CharStateMask ^= EP10CharMask::Aim;
}

void AP10Character::SpawnWeapon()
{
	FActorSpawnParameters Parameters;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Parameters.Owner = this;
	Parameters.Instigator = this;

	Weapon = GetWorld()->SpawnActor<AP10Weapon>(WeaponClass, Parameters);
	check(Weapon)

	const FAttachmentTransformRules Rules = {EAttachmentRule::SnapToTarget, true};
	Weapon->AttachToComponent(GetMesh(), Rules, HandSocketName);
}

void AP10Character::PrintMaskAsString(EP10CharMask Mask)
{
	FString MaskString = "0b";
	uint8 MaskInt = static_cast<uint8>(Mask);
	int32 Block = 128;
	for (int i = 0; i < 8; ++i)
	{
		MaskString.Append(MaskInt / Block == 0 ? "0" : "1");
		MaskInt %= Block;
		Block /= 2;
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("%s"), *MaskString));
}

FVector AP10Character::GetPawnViewLocation() const
{
	if (!CameraComponent)
	{
		return Super::GetPawnViewLocation();
	}
	return CameraComponent->GetComponentLocation();
}