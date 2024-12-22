// Copyright (C) 2023, IKinder

#include "Player/P7Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Component/P7AttributeComponent.h"
#include "Component/P7WallRunComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/Weapon/P7Weapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/CharacterTypes.h"
#include "Widget/P7HUD.h"
#include "Widget/P7PlayerOverlay.h"

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
	check(WallRunComponent);
	AddMappingContext();
	Tags.Add("Player");
	InitOverlayWidget();
}

void AP7Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetJumpState() >= EJS_Single && !WallRunComponent->GetIsWallRunning()) Climb();
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

void AP7Character::Jump()
{
	if (WallRunComponent->StartWallJump()) return;
	Super::Jump();
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
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		CharacterState = ECS_Unequipped;
	}
	EquippedWeapon = Weapon;
	EquippedWeapon->Equip(GetMesh(), BeltSocketName, this, this);
	EquippedWeapon->SwitchWeaponHard(false);
}

void AP7Character::SetupComponents()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>("ViewCameraComponent");
	ViewCamera->SetupAttachment(CameraBoom);
	
	WallRunComponent = CreateDefaultSubobject<UP7WallRunComponent>("WallRunComponent");
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

void AP7Character::Climb()
{
	/* Search any obstacles in front of the player */
	constexpr float MinDistanceToClimb = 75.f; 
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * MinDistanceToClimb;
	FHitResult ForwardHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(ForwardHitResult, Start, End, ECC_Visibility, QueryParams);
	if (!ForwardHitResult.bBlockingHit) return;
	/* Check if obstacle has tag */
	if (!ForwardHitResult.GetActor()->ActorHasTag("WallClimb")) return;
	/* Check if the height of the obstacle is acceptable */
	constexpr float MaxHeightToClimb = 160.f;
	FHitResult DownHitResult;
	End = ForwardHitResult.ImpactPoint + ForwardHitResult.ImpactNormal * -40.f;
	Start = End + FVector(0.f, 0.f, MaxHeightToClimb);
	GetWorld()->LineTraceSingleByChannel(DownHitResult, Start, End, ECC_Visibility);
	if (!DownHitResult.bBlockingHit) return;

	/* Find new acceptable location */
	const float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	if ((DownHitResult.ImpactPoint - GetActorLocation()).Z > CapsuleHalfHeight) return;
	FVector NewLocation = DownHitResult.ImpactPoint + FVector(0.f, 0.f, CapsuleHalfHeight);
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = (ForwardHitResult.ImpactNormal * -1.f).Rotation().Yaw;
	
	/* Move the player root to a new acceptable location */
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), NewLocation, NewRotation, true, true, 0.2f, false, EMoveComponentAction::Move, LatentInfo);

	/* Play Climbing Anim Montage */
	if (ClimbMontage)
	{
		PlayAnimMontage(ClimbMontage);
	}
	SetJumpState(EJS_Landed);
}
