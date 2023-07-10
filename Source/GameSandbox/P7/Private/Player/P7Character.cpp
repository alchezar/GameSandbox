// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P7/Public/Component/P7AttributeComponent.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7/Public/Widget/P7HUD.h"
#include "P7/Public/Widget/P7PlayerOverlay.h"

AP7Character::AP7Character()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupComponents();
	bUseControllerRotationYaw = false;
	OrientToMovement(false);
}

void AP7Character::BeginPlay()
{
	Super::BeginPlay();
	AddMappingContext();
	Tags.Add("Player");
	InitOverlayWidget();
}

void AP7Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::Stand);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ThisClass::Grab);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ThisClass::Roll);
	EnhancedInputComponent->BindAction<ThisClass, bool>(BlockAction, ETriggerEvent::Started, this, &ThisClass::Block, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(BlockAction, ETriggerEvent::Completed, this, &ThisClass::Block, false);
	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Started, this, &ThisClass::Run, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(RunAction, ETriggerEvent::Completed, this, &ThisClass::Run, false);
}

void AP7Character::GetHit(const FVector& HitterLocation)
{
	Super::GetHit(HitterLocation);
	ActionState = EAS_HitReaction;
}

bool AP7Character::GetIsAttaching()
{
	return ActionState == EAS_Attacking;
}

void AP7Character::SetOverlappingItem(AP7Item* Item)
{
	OverlappingItem = Item;
}

void AP7Character::AddCoins(const int32 Coins)
{
	Attributes->AddCoins(Coins);
}

void AP7Character::AddSouls(const int32 Souls)
{
	Attributes->AddSoul(Souls);
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

void AP7Character::OnHitReactEndHandle(USkeletalMeshComponent* MeshComp)
{
	Super::OnHitReactEndHandle(MeshComp);
	ActionState = EAS_Unoccupied;
}

void AP7Character::Grab()
{
	/* Try to find overlapping Weapon items at first*/
	EquipWeapon(Cast<AP7Weapon>(OverlappingItem));
	/* Try to put current weapon away or get it back*/
	if (!EquippedWeapon || !EquipMontage || ActionState == EAS_Attacking) return;
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

void AP7Character::EquipWeapon(AP7Weapon* Weapon)
{
	if (!Weapon) return;
	Weapon->Equip(GetMesh(), BeltSocketName, this, this);
	EquippedWeapon = Weapon;
	EquippedWeapon->SwitchWeaponHard(false);
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

void AP7Character::AddMappingContext() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP7Character::Look(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AP7Character::Move(const FInputActionValue& Value)
{
	if (!Controller || ActionState != EAS_Unoccupied) return;
	StopAnimMontage(AttackMontage);
	OrientToMovement(false);
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
	LastMovementVector = MovementVector;
}

void AP7Character::Stand(const FInputActionValue& Value)
{
	OrientToMovement(true);
	LastMovementVector = FVector2D::ZeroVector;
}

void AP7Character::Block(bool bBlock)
{
	SetIsBlocked(bBlock);
	GetCharacterMovement()->MaxWalkSpeed = bBlock ? 200.f : 600.f;
}

void AP7Character::Run(bool bRun)
{
	if (GetIsBlocked()) return;
	GetCharacterMovement()->MaxWalkSpeed = bRun ? 900.f : 600.f;
}

void AP7Character::Roll()
{
	if (!RollMontage || Attributes->GetCurrentStamina() < StaminaCost) return;
	
	FString SectionString = "RollBack";
	if      (LastMovementVector.X < 0.f) SectionString = "RollLeft";
	else if (LastMovementVector.X > 0.f) SectionString = "RollRight";
	else if	(LastMovementVector.Y > 0.f) SectionString = "RollFront";
	if (CharacterState > ECS_Unequipped) SectionString = "Combat" + SectionString;
	const FName SectionName = FName(SectionString);
	PlayAnimMontage(RollMontage, 1.f, SectionName);
	
	Attributes->UseStamina(StaminaCost);

}

void AP7Character::InitOverlayWidget()
{
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	const AP7HUD* GameHUD = Cast<AP7HUD>(PC->GetHUD());
	if (!GameHUD) return;
	OverlayWidget = GameHUD->GetPlayerOverlay();
	OverlayWidget->ConnectToCharacter(this);
}
