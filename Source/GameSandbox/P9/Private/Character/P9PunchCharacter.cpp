// Copyright (C) 2023, IKinder

#include "P9/Public/Character/P9PunchCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogP9PunchCharacter, All, All)

AP9PunchCharacter::AP9PunchCharacter()
{
	/* Enable tick */
	PrimaryActorTick.bCanEverTick = true;
	/* Set size for collision capsule */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	/* Don`t rotate when the controller rotates. Let that just affect the camera */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	/* Create new and setup current components */
	SetupComponents();
}

void AP9PunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	CheckHardReferences();
	AddMappingContext();
}

void AP9PunchCharacter::SetupComponents()
{
	/* Configure character movement */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	/* Create camera boom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	/* Create camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCameraComponent");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AP9PunchCharacter::CheckHardReferences()
{
	check(DefaultContext)
	check(MoveAction)
	check(LookAction)
	check(JumpAction)
}

void AP9PunchCharacter::AddMappingContext()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP9PunchCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP9PunchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;
	/* Default input actions */
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	/* Attack functionality */
	EnhancedInputComponent->BindAction<ThisClass, bool>(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack, true);
	EnhancedInputComponent->BindAction<ThisClass, bool>(AttackAction, ETriggerEvent::Completed, this, &ThisClass::Attack, false);
}

void AP9PunchCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AP9PunchCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AP9PunchCharacter::Attack(bool bAttack)
{
	Log(EP9LogLevel::INFO, __FUNCTION__);
	Log(EP9LogLevel::INFO, bAttack ? "Attack start" : "Attack end");
}

void AP9PunchCharacter::Log(const EP9LogLevel LogLevel, const FString& Message)
{
	Log(LogLevel, Message, EP9LogOutput::ALL);
}

void AP9PunchCharacter::Log(const EP9LogLevel LogLevel, const FString& Message, const EP9LogOutput LogOutput)
{
	/* Only print when screen is selected and GEngine object is available */
	if (GEngine &&(LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::SCREEN))
	{
		/* Default color */
		FColor LogColor;
		/* Flip the color based on the type */
		if (LogLevel == EP9LogLevel::TRACE)        LogColor = FColor::Green;
		else if (LogLevel == EP9LogLevel::DEBUG)   LogColor = FColor::Cyan;
		else if (LogLevel == EP9LogLevel::INFO)    LogColor = FColor::White;
		else if (LogLevel == EP9LogLevel::WARNING) LogColor = FColor::Yellow;
		else if (LogLevel == EP9LogLevel::ERROR)   LogColor = FColor::Red;
		/* Print the message and leave it on screen */
		GEngine->AddOnScreenDebugMessage(-1, 5.f, LogColor, Message);
	}
	if (LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::LOG)
	{
		// flip the message type based on error level
		switch (LogLevel)
		{
		case EP9LogLevel::TRACE:   UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *Message); break;
		case EP9LogLevel::DEBUG:   UE_LOG(LogTemp, Verbose, TEXT("%s"), *Message);     break;
		case EP9LogLevel::INFO:    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);         break;
		case EP9LogLevel::WARNING: UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);     break;
		case EP9LogLevel::ERROR:   UE_LOG(LogTemp, Error, TEXT("%s"), *Message);       break;
		default:                   UE_LOG(LogTemp, Log, TEXT("%s"), *Message);         break;
		}
	}
}