// Copyright (C) 2023, IKinder

#include "P8/Public/Pawn/P8Kart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector.h"
#include "P8/Public/Component/P8MovementComponent.h"
#include "P8/Public/Component/P8ReplicatorComponent.h"

AP8Kart::AP8Kart()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicatingMovement(false);
	
	SetupComponents();
}

void AP8Kart::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(KartMesh)
	check(CameraBoom)
	check(CameraView)
	check(MoveComp)
	check(RepComp)
	
	RepComp->Setup(MoveComp, MeshOffsetRoot);
}

void AP8Kart::BeginPlay()
{
	Super::BeginPlay();
	AddMappingContext();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1.f;
	}
}

void AP8Kart::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP8Kart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent)
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::Move);
}

void AP8Kart::SetupComponents()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComponent");
	SetRootComponent(BoxCollision);
	BoxCollision->SetBoxExtent(FVector(230.f, 90.f, 75.f));
	BoxCollision->SetCollisionProfileName("BlockAllDynamic");

	MeshOffsetRoot = CreateDefaultSubobject<USceneComponent>("MeshOffsetRootComponent");
	MeshOffsetRoot->SetupAttachment(RootComponent);

	KartMesh = CreateDefaultSubobject<USkeletalMeshComponent>("KartSkeletalMesh");
	KartMesh->SetupAttachment(MeshOffsetRoot);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(MeshOffsetRoot);

	CameraView = CreateDefaultSubobject<UCameraComponent>("CameraViewComponent");
	CameraView->SetupAttachment(CameraBoom);

	MoveComp = CreateDefaultSubobject<UP8MovementComponent>("KartMovementComponent");
	RepComp = CreateDefaultSubobject<UP8ReplicatorComponent>("KartMovementReplicatorComponent");
}

void AP8Kart::AddMappingContext() const
{
	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;
	auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!LocalPlayerSubsystem) return;
	LocalPlayerSubsystem->AddMappingContext(DefaultContext, 0);
}

void AP8Kart::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();
	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}

void AP8Kart::Move(const FInputActionValue& Value)
{
	MoveComp->Move(Value.Get<FVector2D>());
}
