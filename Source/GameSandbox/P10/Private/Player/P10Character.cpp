// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Weapon/P10Projectile.h"

AP10Character::AP10Character()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	ArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("GunMeshSkeletalComponent");
	GunMeshComponent->SetupAttachment(GetMesh(), HandSocketName);
}

void AP10Character::BeginPlay()
{
	Super::BeginPlay();
	check(ProjectileClass)
	check(FireAnimation)
	check(FireSound)

	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP10Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bCarryingObjective);
}

void AP10Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::Fire);
}

void AP10Character::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AP10Character::Look(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D LookingVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookingVector.X);
	AddControllerPitchInput(LookingVector.Y);
}

void AP10Character::Fire()
{
	/* Spawn Projectile actor. */
	const FVector MuzzleLocation = GunMeshComponent->GetSocketLocation(MuzzleSocketName);
	const FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation(MuzzleSocketName);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AP10Projectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
	/* Play sound and fire animation. */
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLocation, FRotator::ZeroRotator);
	PlayAnimMontage(FireAnimation);
}

void AP10Character::Server_Fire_Implementation()
{}

bool AP10Character::Server_Fire_Validate()
{
	return true;
}
