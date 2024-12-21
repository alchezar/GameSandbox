// Copyright (C) 2023, IKinder

#include "Player/ER_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ER_GameModeBase.h"

AER_Character::AER_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength         = 350.f;
	CameraArm->SocketOffset            = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void AER_Character::BeginPlay()
{
	Super::BeginPlay();

	bDead = false;
	if (const auto PlayerController = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	GameMode = Cast<AER_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		LaneMidLocations = GameMode->GetLaneMidLocations();
	}

	CurrentLaneIndex    = 1;
	CurrentLanePosition = 0.f;
	DefaultHalfHeight   = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	PlayerStartLocation = GetActorLocation();
}

void AER_Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward();
}

void AER_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const auto EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(ActionLeft, ETriggerEvent::Started, this, &ThisClass::MoveLeft);
		EnhancedInput->BindAction(ActionRight, ETriggerEvent::Started, this, &ThisClass::MoveRight);
		EnhancedInput->BindAction(ActionDown, ETriggerEvent::Started, this, &ThisClass::MoveDown);
		EnhancedInput->BindAction(ActionJump, ETriggerEvent::Started, this, &ThisClass::Jump);
	}
}

void AER_Character::MoveForward()
{
	if (bDead) return;
	FRotator ControlRotation = FRotator::ZeroRotator;
	ControlRotation.Yaw      = GetControlRotation().Yaw;
	AddMovementInput(ControlRotation.Vector());
}

void AER_Character::MoveLeft()
{
	if (bDead) return;
	CurrentLaneIndex = FMath::Clamp(--CurrentLaneIndex, 0, LaneMidLocations.Num() - 1);
	GetWorldTimerManager().SetTimer(SlideTimer, this, &ThisClass::SmoothlyChangeLane, GetWorld()->DeltaTimeSeconds, true);
}

void AER_Character::MoveRight()
{
	if (bDead) return;
	CurrentLaneIndex = FMath::Clamp(++CurrentLaneIndex, 0, LaneMidLocations.Num() - 1);
	GetWorldTimerManager().SetTimer(SlideTimer, this, &ThisClass::SmoothlyChangeLane, GetWorld()->DeltaTimeSeconds, true);
}

void AER_Character::MoveDown()
{
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->AddImpulse(Impulse, true);
	}
	// Check if already sliding
	if (GetWorldTimerManager().IsTimerActive(RunSlideTimer))
	{
		GetWorldTimerManager().ClearTimer(RunSlideTimer);
		StandUp();
	}

	// Slide
	PlayAnimMontage(SlideAnimation);
	const float NewHalfHeight = DefaultHalfHeight / 2;
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -NewHalfHeight));
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f + NewHalfHeight);

	GetWorldTimerManager().SetTimer(RunSlideTimer, this, &ThisClass::StandUp, 0.7f);
}

void AER_Character::StandUp()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultHalfHeight);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -DefaultHalfHeight));
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	// TODO: Find how to fix little camera jitter after stand up
}

void AER_Character::Jump()
{
	Super::Jump();

	FTimerHandle   JumpTimer;
	FTimerDelegate JumpDelegate;
	JumpDelegate.BindLambda([&]()
	{
		GetCharacterMovement()->AddImpulse(Impulse, true);
		StopJumping();
	});
	GetWorldTimerManager().SetTimer(OUT JumpTimer, JumpDelegate, JumpTime, false);
}

void AER_Character::SmoothlyChangeLane()
{
	const float Target  = LaneMidLocations[CurrentLaneIndex].Y;
	CurrentLanePosition = FMath::FInterpTo(CurrentLanePosition, Target, GetWorld()->DeltaTimeSeconds, ChangeLineSpeed);

	FVector Location = GetRootComponent()->GetRelativeLocation();
	Location.Y       = CurrentLanePosition;
	GetRootComponent()->SetRelativeLocation(FVector(Location));

	if (FMath::Abs(CurrentLanePosition - Target) < 1.f)
	{
		if (!GetWorld()) return;

		GetWorldTimerManager().ClearTimer(SlideTimer);
		Location.Y = Target;
		GetRootComponent()->SetRelativeLocation(FVector(Location));
	}
}

void AER_Character::Death()
{
	if (!GetWorld() || !ExplosionParticle || !ExplosionSound) return;

	bDead = true;
	GetCharacterMovement()->DisableMovement();	
	GetCharacterMovement()->MovementState.bCanJump = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(3.f));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

	GetWorldTimerManager().ClearTimer(SlideTimer);

	GameMode->DecreaseLives();

	FTimerHandle DeathTimer;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &ThisClass::NextLife, 2.f);
}

void AER_Character::NextLife()
{
	GameMode->StartFromBegin(this);
}

bool AER_Character::IsDead() const
{
	return bDead;
}

void AER_Character::AddCoin()
{
	GameMode->AddCoin();
}

void AER_Character::Resurrect()
{
	bDead = false;
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -DefaultHalfHeight), FRotator(0.f, -90.f, 0.f));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(PlayerStartLocation);
	GetCharacterMovement()->MovementState.bCanJump = true;	
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}
