// Copyright (C) 2023, IKinder

#include "P7/Public/Pawn/P7Bird.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

AP7Bird::AP7Bird()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	SetupComponents();
}

void AP7Bird::BeginPlay()
{
	Super::BeginPlay();
	AddMappingContext();
}

void AP7Bird::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Bird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MovePawn);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookPawn);
}

void AP7Bird::SetupComponents()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	ViewCamera->SetupAttachment(CameraBoom);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BirdMeshComponent");
	BirdMesh->SetupAttachment(RootComponent);
	BirdMesh->SetRelativeTransform(FTransform(FRotator(0.f, 90.f, 0.f), FVector(0.f, 0.f, -10.f)));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovementComponent");
}

void AP7Bird::AddMappingContext() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP7Bird::MovePawn(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void AP7Bird::LookPawn(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
