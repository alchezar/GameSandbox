// Copyright (C) 2023, IKinder

#include "P8/Public/Pawn/P8Kart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector.h"
#include "Net/UnrealNetwork.h"

AP8Kart::AP8Kart()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetupComponents();
}

void AP8Kart::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AP8Kart::BeginPlay()
{
	Super::BeginPlay();
	check(KartMesh)
	check(CameraBoom)
	check(CameraView)
	AddMappingContext();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1.f;
	}
}

void AP8Kart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FP8Move CurrentMove = FP8Move(MoveAlpha, TurnAlpha, DeltaTime, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	/* We are the client */
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		SimulateMove(CurrentMove);
		UnacknowledgeMoves.Add(CurrentMove);
		Server_SendMove(CurrentMove);
	}
	/* We are the server and in control of the pawn */
	if (HasAuthority() && IsLocallyControlled())
	{
		Server_SendMove(CurrentMove);
	}
	/* Client prediction */
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		SimulateMove(ServerState.LastMove);
	}
	// DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), UEnum::GetValueAsString(GetLocalRole()).RightChop(5), this, FColor::White, 0.f);
}

void AP8Kart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ServerState);
}

void AP8Kart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent)
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction<ThisClass, bool>(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::GeneralMove, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(MoveAction, ETriggerEvent::Completed, this, &ThisClass::GeneralMove, false);
}

void AP8Kart::SetupComponents()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComponent");
	SetRootComponent(BoxCollision);
	BoxCollision->SetBoxExtent(FVector(230.f, 90.f, 75.f));
	BoxCollision->SetCollisionProfileName("BlockAllDynamic");

	KartMesh = CreateDefaultSubobject<USkeletalMeshComponent>("KartSkeletalMesh");
	KartMesh->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomSpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);

	CameraView = CreateDefaultSubobject<UCameraComponent>("CameraViewComponent");
	CameraView->SetupAttachment(CameraBoom);
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

void AP8Kart::GeneralMove(const FInputActionValue& Value, bool bPressed)
{
	Move(Value.Get<FVector2D>());
}

void AP8Kart::Server_SendMove_Implementation(FP8Move ServerMove)
{
	// Move(FVector2D(ServerMove.TurnAlpha, ServerMove.MoveAlpha));
	SimulateMove(ServerMove);
	ServerState.LastMove = ServerMove;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
}

bool AP8Kart::Server_SendMove_Validate(FP8Move ServerMove)
{
	return FMath::Abs(ServerMove.MoveAlpha) <= 1.f && FMath::Abs(ServerMove.TurnAlpha) <= 1.f;
}

void AP8Kart::Move(const FVector2D Value)
{
	MoveAlpha = Value.Y;
	TurnAlpha = Value.X;
}

void AP8Kart::OnRep_ServerState()
{
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;
	
	ClearAcknowledgeMoves(ServerState.LastMove);
	for (const FP8Move& UnacknowledgeMove : UnacknowledgeMoves)
	{
		SimulateMove(UnacknowledgeMove);
	}
}

void AP8Kart::SimulateMove(const FP8Move& TheMove)
{
	check(Mass > 0.f)
	/* Calculate Resistances */
	const double NormalForce = Mass * (-GetWorld()->GetGravityZ() / 100);
	const FVector RollingResistance = RollingCoefficient * NormalForce * Velocity.GetSafeNormal();
	const FVector AirResistance = DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
	/* Find kart Velocity */
	const FVector Force = GetActorForwardVector() * MaxMoveForce * TheMove.MoveAlpha - AirResistance - RollingResistance;
	const FVector Acceleration = Force / Mass;
	Velocity += Acceleration * TheMove.DeltaTime; 
	FHitResult HitResult;
	AddActorWorldOffset(Velocity * 100.0 * TheMove.DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
	{
		Velocity = FVector::ZeroVector;
	}

	const float RotationAngle = (GetActorForwardVector().Dot(Velocity) * TheMove.DeltaTime) / MinTurnRadius * TheMove.TurnAlpha;
	const FQuat RotationDelta = FQuat(GetActorUpVector(), RotationAngle);
	AddActorWorldRotation(RotationDelta, true);
	Velocity = RotationDelta.RotateVector(Velocity);
}

void AP8Kart::ClearAcknowledgeMoves(const FP8Move LastMove)
{
	TArray<FP8Move> NewMoves;
	for (const FP8Move& UnacknowledgeMove : UnacknowledgeMoves)
	{
		if (UnacknowledgeMove.Time > LastMove.Time)
		{
			NewMoves.Add(UnacknowledgeMove);
		}
	}
	UnacknowledgeMoves = NewMoves;
}
