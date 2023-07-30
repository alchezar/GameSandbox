// Copyright (C) 2023, IKinder

#include "P8/Public/Pawn/P8Kart.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

void AP8Kart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
	{
		Server_MovementUpdate(DeltaTime);
	}
	MovementUpdate(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f),UEnum::GetValueAsString(GetLocalRole()).RightChop(5), this, FColor::White, 0.f);
}

void AP8Kart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent)
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::LocalMove);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::LocalMove);
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

void AP8Kart::LocalMove(const FInputActionValue& Value)
{
	if (!HasAuthority())
	{
		Server_Move(Value);
	}
	Move(Value);
}

void AP8Kart::Server_Move_Implementation(const FInputActionValue& Value)
{
	Move(Value);
}

void AP8Kart::Move(const FInputActionValue& Value)
{
	const FVector2D MovementDirection = Value.Get<FVector2D>();
	MoveAlpha = MovementDirection.Y;
	TurnAlpha = MovementDirection.X;
}

bool AP8Kart::Server_Move_Validate(const FInputActionValue& Value)
{
	const float AbsX = FMath::Abs(Value.Get<FVector2D>().X);
	const float AbsY = FMath::Abs(Value.Get<FVector2D>().Y);
	return AbsX <= 1.f && AbsY <= 1.f;
}

void AP8Kart::MovementUpdate(const float DeltaTime)
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
	AddActorWorldOffset(Velocity * 100.f * DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
	{
		Velocity = FVector::ZeroVector;
	}

	const float RotationAngle = (GetActorForwardVector().Dot(Velocity) * DeltaTime) / MinTurnRadius * TurnAlpha;
	const FQuat RotationDelta = FQuat(GetActorUpVector(), RotationAngle);
	AddActorWorldRotation(RotationDelta, true);

	Velocity = RotationDelta.RotateVector(Velocity);
}
