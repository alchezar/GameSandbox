// Copyright (C) 2023, IKinder

#include "ER_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	if (const auto PlayerController = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
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
	FRotator ControlRotation = FRotator::ZeroRotator;
	ControlRotation.Yaw      = GetControlRotation().Yaw;
	AddMovementInput(ControlRotation.Vector());
}

void AER_Character::MoveLeft()
{
	SmoothSlide(-1);
}

void AER_Character::MoveRight()
{
	SmoothSlide(1);
}

void AER_Character::MoveDown()
{ }

void AER_Character::Jump()
{
	Super::Jump();

	FTimerHandle JumpTimer;
	FTimerDelegate JumpDelegate;
	JumpDelegate.BindLambda([&]()
	{
		StopJumping();
	});
	GetWorldTimerManager().SetTimer(OUT JumpTimer, JumpDelegate, JumpTime, false);
}

void AER_Character::SmoothSlide(int32 SideDirection)
{
	SideDirection = FMath::Clamp(SideDirection, -1, 1);
	TargetOffset  = FMath::Clamp(TargetOffset + Distance * SideDirection, -Distance, Distance);

	FTimerDelegate SlideDelegate;
	SlideDelegate.BindLambda([&]()
	{
		FVector Location = GetRootComponent()->GetRelativeLocation();
		CurrentOffset    = FMath::FInterpTo(CurrentOffset, TargetOffset, GetWorld()->DeltaTimeSeconds, 10.f);
		Location.Y       = CurrentOffset;
		GetRootComponent()->SetRelativeLocation(FVector(Location));

		if (FMath::Abs(CurrentOffset - TargetOffset) < 1.f)
		{
			GetWorldTimerManager().ClearTimer(SlideTimer);
			Location.Y = TargetOffset;
			GetRootComponent()->SetRelativeLocation(FVector(Location));
		}
	});
	GetWorldTimerManager().SetTimer(SlideTimer, SlideDelegate, GetWorld()->DeltaTimeSeconds, true);
}
