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
	SetupComponents();
}

void AP8Kart::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlap);
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
	MovementUpdate(DeltaTime);
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

void AP8Kart::Move(const FInputActionValue& Value)
{
	const FVector2D MovementDirection = Value.Get<FVector2D>();
	MoveAlpha = MovementDirection.Y;
	TurnAlpha = MovementDirection.X;
}

void AP8Kart::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Hello")));
}

void AP8Kart::MovementUpdate(const float DeltaTime)
{
	check(Mass > 0.f)
	const FVector AirResistance = DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
	const FVector Force = GetActorForwardVector() * MaxMoveForce * MoveAlpha - AirResistance;
	const FVector Acceleration = Force / Mass;
	Velocity += Acceleration * DeltaTime;
	
	FHitResult HitResult;
	AddActorWorldOffset(Velocity * 100.f * DeltaTime, true, &HitResult);
	if (HitResult.bBlockingHit)
	{
		Velocity = FVector::ZeroVector;
	}

	const float BackRotationFix = MoveAlpha < 0 ? -1.f : 1.f;
	const float RotationAngle = MaxTurnPerSecond * TurnAlpha * BackRotationFix * DeltaTime;
	const FQuat RotationDelta = FQuat(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));
	AddActorWorldRotation(RotationDelta, true);
	Velocity = RotationDelta.RotateVector(Velocity);
}
