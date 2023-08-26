// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Util/P10Library.h"
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

	UP10Library::PrintStateMask(CharStateMask);
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
	if (UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Crouch)) return;
	UP10Library::BitflagAdd(CharStateMask, EP10CharMask::Jump);
}

void AP10Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UP10Library::BitflagRemove(CharStateMask, EP10CharMask::Jump);
}

void AP10Character::CrouchInput()
{
	UP10Library::BitflagToggle(CharStateMask, EP10CharMask::Crouch);
	UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Crouch) ? Crouch() : UnCrouch();
}

void AP10Character::FireInput(const bool bShoot)
{
	UP10Library::BitflagFromBool(CharStateMask, EP10CharMask::Shoot, bShoot);
	if (!bShoot) return;

	Server_Fire();
}

void AP10Character::Server_Fire_Implementation()
{
	Weapon->StartFire();
}

bool AP10Character::Server_Fire_Validate()
{
	return true;
}

void AP10Character::AimInput(const bool bAim)
{
	UP10Library::BitflagFromBool(CharStateMask, EP10CharMask::Aim, bAim);
	constexpr float DefaultFieldOfView = 90.f;
	constexpr float ZoomedFieldOfView = 45.f;
	TargetFOV = bAim ? ZoomedFieldOfView : DefaultFieldOfView;
	// CameraComponent->SetFieldOfView(TargetFOV);
	GetWorld()->GetTimerManager().SetTimer(ZoomTimer, this, &ThisClass::ZoomSmoothlyHandle, GetWorld()->GetDeltaSeconds(), true);
}

void AP10Character::ZoomSmoothlyHandle()
{
	const float InterFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(), 10.f);
	CameraComponent->SetFieldOfView(InterFOV);
	
	if (FMath::IsNearlyEqual(CameraComponent->FieldOfView, TargetFOV, 1.f))
	{
		GetWorld()->GetTimerManager().ClearTimer(ZoomTimer);
	}
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

FVector AP10Character::GetPawnViewLocation() const
{
	if (!CameraComponent)
	{
		return Super::GetPawnViewLocation();
	}
	return CameraComponent->GetComponentLocation();
}
