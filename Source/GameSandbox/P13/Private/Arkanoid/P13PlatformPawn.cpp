// Copyright © 2024, IKinder

#include "P13/Public/Arkanoid/P13PlatformPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "P13/Public/Arkanoid/P13Ball.h"

AP13PlatformPawn::AP13PlatformPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformStaticMeshComponent");
	PlatformMesh->SetupAttachment(RootComponent);

	BallSpawnPoint = CreateDefaultSubobject<USceneComponent>("BallSpawnPointSceneComponent");
	BallSpawnPoint->SetupAttachment(PlatformMesh);
}

void AP13PlatformPawn::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	CreateBall();
}

void AP13PlatformPawn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13PlatformPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(SideAction, ETriggerEvent::Triggered, this, &ThisClass::OnSideInput);
}

void AP13PlatformPawn::AddDefaultMappingContext()
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

void AP13PlatformPawn::OnSideInput(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	const FVector SideMoveVector = FVector(0.f, MoveVector.X * PlatformSpeed, 0.f); 
	AddActorWorldOffset(SideMoveVector, true);
}

void AP13PlatformPawn::CreateBall()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CachedBall = GetWorld()->SpawnActor<AP13Ball>(BallClass, BallSpawnPoint->GetComponentTransform(), SpawnParams);
	if (!CachedBall.IsValid())
	{
		return;
	}
	const FDelegateHandle OnBallDeadDelegate = CachedBall->OnBallDead.AddUObject(this, &ThisClass::OnBallDeadHandle);
	CachedBall->AddDelegateHandle(OnBallDeadDelegate);
}

void AP13PlatformPawn::OnBallDeadHandle()
{
	CreateBall();
}
