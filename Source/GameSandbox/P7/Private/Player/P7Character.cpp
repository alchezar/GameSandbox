// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
}

void AP7Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ThisClass::Grab);
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

void AP7Character::AddMappingContext() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP7Character::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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
	if (AP7Weapon* Weapon = Cast<AP7Weapon>(OverlappingItem))
	{
		Weapon->Equip(GetMesh(), SocketName);
		CharacterState = ECharacterState::ECS_OneHanded;
	}
}

void AP7Character::OrientToMovement(const bool bOrient) const
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp) return;
	MovementComp->bOrientRotationToMovement = bOrient;
	MovementComp->bUseControllerDesiredRotation = !bOrient;
}
void AP7Character::Jump()
{
	Super::Jump();
	
	if (!GetCharacterMovement()->IsFalling() || bDoubleJump) return;
	bDoubleJump = true;
	LaunchCharacter(GetVelocity() + FVector(0.f, 0.f, 700.f), true, true);
}

void AP7Character::Landed(const FHitResult& Hit)
{
	bDoubleJump = false;
	Super::Landed(Hit);
}

void AP7Character::SetOverlappingItem(AP7Item* Item)
{
	OverlappingItem = Item;
}
