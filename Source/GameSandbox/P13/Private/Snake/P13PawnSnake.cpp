// Copyright © 2024, IKinder

#include "P13/Public/Snake/P13PawnSnake.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "P13/Public/Snake/P13SnakeBase.h"

AP13PawnSnake::AP13PawnSnake()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>("PawnCameraComponent");
	PawnCamera->SetupAttachment(RootComponent);
	PawnCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void AP13PawnSnake::BeginPlay()
{
	Super::BeginPlay();
	AddDefaultMappingContext();

	CreateSnakeActor();
}

void AP13PawnSnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13PawnSnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
}

void AP13PawnSnake::AddDefaultMappingContext()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}

	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP13PawnSnake::MoveInput(const FInputActionValue& Value)
{
	if (!CachedSnake.IsValid())
	{
		return;
	}
	const FVector2D MoveVector = Value.Get<FVector2D>();
	EP13MoveDirection MoveDirection = CachedSnake->GetMoveDirection();
	
	if (MoveVector.Y > 0.f && MoveDirection != EP13MoveDirection::Down)
	{
		MoveDirection = EP13MoveDirection::Up;
	}
	if (MoveVector.Y < 0.f && MoveDirection != EP13MoveDirection::Up)
	{
		MoveDirection = EP13MoveDirection::Down;
	}
	else if (MoveVector.X < 0.f && MoveDirection != EP13MoveDirection::Right)
	{
		MoveDirection = EP13MoveDirection::Left;
	}
	else if (MoveVector.X > 0.f && MoveDirection != EP13MoveDirection::Left)
	{
		MoveDirection = EP13MoveDirection::Right;
	}

	CachedSnake->SetMoveDirection(MoveDirection);
}

void AP13PawnSnake::CreateSnakeActor()
{
	CachedSnake = GetWorld()->SpawnActor<AP13SnakeBase>(SnakeClass, FTransform::Identity);
}
