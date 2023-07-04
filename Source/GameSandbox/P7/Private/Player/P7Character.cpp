// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7/Public/Player/CharacterTypes.h"

AP7Character::AP7Character()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupComponents();
	bUseControllerRotationYaw = false;
	OrientToMovement(true);
}

void AP7Character::BeginPlay()
{
	Super::BeginPlay();
	AddMappingContext();
	Tags.Add("Player");
}

void AP7Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Character::AddMappingContext() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP7Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ThisClass::Grab);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
}

void AP7Character::SetupComponents()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	ViewCamera->SetupAttachment(CameraBoom);
}

void AP7Character::OrientToMovement(const bool bOrient) const
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp) return;
	MovementComp->bOrientRotationToMovement = bOrient;
	MovementComp->bUseControllerDesiredRotation = !bOrient;
}

void AP7Character::Move(const FInputActionValue& Value)
{
	if (!Controller || ActionState != EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AP7Character::Look(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AP7Character::Grab()
{
	/* Try to find overlapping Weapon items at first*/
	if (AP7Weapon* Weapon = Cast<AP7Weapon>(OverlappingItem))
	{
		Weapon->Equip(GetMesh(), BeltSocketName, this, this);
		EquippedWeapon = Weapon;
		EquippedWeapon->SwitchWeaponHard(false);
	}
	/* Try to put current weapon away or get it back*/
	if (EquippedWeapon && ActionState != EAS_Attacking && EquipMontage)
	{
		FName SectionName;
		if (CharacterState == ECS_Unequipped)
		{
			SectionName = FName("Equip");
			CharacterState = EquippedWeapon->GetWeaponState();
		}
		else
		{
			SectionName = FName("Unequip");
			CharacterState = ECS_Unequipped;
		}
		PlayAnimMontage(EquipMontage, 1.f, SectionName);
		ActionState = EAS_Equipping;
	}
}

void AP7Character::GetHit(const FVector& ImpactPoint)
{
	Super::GetHit(ImpactPoint);
}

bool AP7Character::GetIsAttaching()
{
	return ActionState == EAS_Attacking;
}

bool AP7Character::CanAttack()
{
	return EquippedWeapon && ActionState == EAS_Unoccupied && CharacterState != ECS_Unequipped;
}

void AP7Character::Attack()
{
	Super::Attack();
	if (!CanAttack()) return;
	ActionState = EAS_Attacking;
}

void AP7Character::SetOverlappingItem(AP7Item* Item)
{
	OverlappingItem = Item;
}

void AP7Character::OnAttackEndHandle(USkeletalMeshComponent* MeshComp)
{
	Super::OnAttackEndHandle(MeshComp);
	ActionState = EAS_Unoccupied;
}

void AP7Character::OnBeamTurningHandle(USkeletalMeshComponent* MeshComp)
{
	EquippedWeapon->SwitchWeapon(CharacterState != ECS_Unequipped);
}

void AP7Character::OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp)
{
	if (EquippedWeapon && CharacterState == ECS_Unequipped)
	{
		EquippedWeapon->AttachToBelt(GetMesh(), BeltSocketName);
	}
	else
	{
		EquippedWeapon->AttachToHand(GetMesh(), HandSocketName);
	}
}
