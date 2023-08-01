// Copyright (C) 2023, IKinder

#include "P8/Public/Pawn/P8Kart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	if (APlayerController* PC = GetLocalViewingPlayerController())
	{
		PossessedBy(PC);
	}
}

void AP8Kart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementUpdate(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f),UEnum::GetValueAsString(GetLocalRole()).RightChop(5), this, FColor::White, 0.f);
}

void AP8Kart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, RepLocation);
	DOREPLIFETIME(ThisClass, RepRotation);
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
	const FVector2D MovementDirection = Value.Get<FVector2D>();
	
	if (!HasAuthority())
	{
		Server_Move(MovementDirection);
	}
	Move(MovementDirection);
}

void AP8Kart::Server_Move_Implementation(const FVector2D Value)
{
	Multicast_Move(Value);
}

void AP8Kart::Multicast_Move_Implementation(const FVector2D Value)
{
	Move(Value);
}

void AP8Kart::Move(const FVector2D Value)
{
	MoveAlpha = Value.Y;
	TurnAlpha = Value.X;
}

void AP8Kart::MovementUpdate(float DeltaTime)
{
	check(Mass > 0.f)
	/* Resistances */
	const float NormalForce = Mass * (-GetWorld()->GetGravityZ() / 100);
	const FVector RollingResistance = RollingCoefficient * NormalForce * Velocity.GetSafeNormal();
	const FVector AirResistance = DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
	/* Velocity */
	const FVector Force = GetActorForwardVector() * MaxMoveForce * MoveAlpha - AirResistance - RollingResistance;
	const FVector Acceleration = Force / Mass;
	Velocity += Acceleration * DeltaTime;
	FHitResult HitResult;
	AddActorWorldOffset(Velocity * 100.0 * DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
	{
		Velocity = FVector::ZeroVector;
	}

	const float RotationAngle = (GetActorForwardVector().Dot(Velocity) * DeltaTime) / MinTurnRadius * TurnAlpha;
	const FQuat RotationDelta = FQuat(GetActorUpVector(), RotationAngle);
	AddActorWorldRotation(RotationDelta, true);
	Velocity = RotationDelta.RotateVector(Velocity);

	if (HasAuthority())
	{
		RepLocation = GetActorLocation();
		RepRotation = GetActorRotation();
	}
	else
	{
		SetActorLocation(RepLocation);
		SetActorRotation(RepRotation);
	}
}
