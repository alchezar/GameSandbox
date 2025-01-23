// Copyright (C) 2023, IKinder

#include "Player/P10Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/P10HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Game/P10GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Util/P10Library.h"
#include "Weapon/P10Weapon.h"

AP10Character::AP10Character()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("PawnNoiseEmitterComponent");

	HealthComponent = CreateDefaultSubobject<UP10HealthComponent>("HealthActorComponent");

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	JumpMaxCount = 2;
}

void AP10Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
	}
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
		/* If there is no AimOffset in AnimInstance, replace mesh rotation pitch with replicated decompressed RemoteViewPitch. */
		FRotator NewRot = ArmComponent->GetRelativeRotation();
		NewRot.Pitch = RemoteViewPitch * 360.f / 256.f;
		ArmComponent->SetRelativeRotation(NewRot);
	}

	if (Cast<APlayerController>(GetController()))
	{
		UP10Library::PrintStateMask(CharStateMask, 8);
		UP10Library::DrawAmmoInfo(this, Weapon);
	}
}

void AP10Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bCarryingObjective);
	DOREPLIFETIME(ThisClass, Weapon);
	DOREPLIFETIME(ThisClass, CharStateMask)
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

	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::ReloadInput);
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

	if (!Weapon) return;
	bShoot ? Weapon->StartFire() : Weapon->StopFire();
}

void AP10Character::AimInput(const bool bAim)
{
	UP10Library::BitflagFromBool(CharStateMask, EP10CharMask::Aim, bAim);
	constexpr float DefaultFieldOfView = 90.f;
	constexpr float ZoomedFieldOfView = 45.f;
	TargetFOV = bAim ? ZoomedFieldOfView : DefaultFieldOfView;
	GetWorld()->GetTimerManager().SetTimer(ZoomTimer, this, &ThisClass::ZoomSmoothlyHandle, GetWorld()->GetDeltaSeconds(), true);
}

void AP10Character::ReloadInput()
{
	Weapon->TryReload();
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
	if (!HasAuthority()) return;
	
	FActorSpawnParameters Parameters;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Parameters.Owner = this;
	Parameters.Instigator = this;

	Weapon = GetWorld()->SpawnActor<AP10Weapon>(WeaponClass, Parameters);
	check(Weapon)

	const FAttachmentTransformRules Rules = {EAttachmentRule::SnapToTarget, true};
	Weapon->AttachToComponent(GetMesh(), Rules, HandSocketName);
	Weapon->OnReload.AddUObject(this, &ThisClass::OnWeaponReloadHandle);
}

void AP10Character::OnWeaponReloadHandle(APawn* WeaponOwner, bool bAccess)
{
	FireInput(false);
}

FVector AP10Character::GetPawnViewLocation() const
{
	if (!CameraComponent)
	{
		return Super::GetPawnViewLocation();
	}
	return CameraComponent->GetComponentLocation();
}

void AP10Character::OnHealthChangedHandle(UP10HealthComponent* Component, float Health, float Delta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0)
	{
		CharStateMask = EP10CharMask::Dead;
		if (AP10GameMode* GameMode = Cast<AP10GameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->RestartAttempt(this);
		}
		Multicast_OnDeath();
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString::Printf(TEXT("%s Health: %f"), *GetName(), Health));
}

void AP10Character::Multicast_OnDeath_Implementation()
{
		/* Ragdoll */
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->MovementState.bCanJump = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.f);
}
