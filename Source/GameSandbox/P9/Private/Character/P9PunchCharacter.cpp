// Copyright (C) 2023, IKinder

#include "P9/Public/Character/P9PunchCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P9/Public/AnimNotify/P9NotifyWindowPunch.h"

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
	InitAnimNotifies();
}

void AP9PunchCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	/* Punch collisions */
	LeftFistCollision = CreateDefaultSubobject<USphereComponent>("LeftFistCollisionComponent");
	LeftFistCollision->SetupAttachment(GetMesh(), "SupportPoint");
	LeftFistCollision->SetSphereRadius(10.f);
	LeftFistCollision->AddLocalOffset(FVector(6.f, 2.f, 0.f));
	LeftFistCollision->SetCollisionProfileName("NoCollision");
	
	RightFistCollision = CreateDefaultSubobject<USphereComponent>("RightFistCollisionComponent");
	RightFistCollision->SetupAttachment(GetMesh(), "GripPoint");
	RightFistCollision->SetSphereRadius(10.f);;
	RightFistCollision->AddLocalOffset(FVector(-2.f, 1.f, -1.f));
	RightFistCollision->SetCollisionProfileName("NoCollision");
}

void AP9PunchCharacter::CheckHardReferences()
{
	/* Check Inputs*/
	check(DefaultContext)
	check(MoveAction)
	check(LookAction)
	check(JumpAction)
	
	/* Check Montages */
	check(MeleeFistAttackMontage)
}

void AP9PunchCharacter::AddMappingContext()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
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
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
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

void AP9PunchCharacter::Attack()
{
	/* Prevent spam multiple attacks. */
	if (CharState >= EP9CharState::ATTACKING) return;
	/* Find first montage section name and chop the last symbol, to get the default section name. */
	const FString MontageSectionName = MeleeFistAttackMontage->GetSectionName(0).ToString().LeftChop(1);
	/* Generate random number */
	const int MontageSectionIndex = FMath::RandRange(0, MeleeFistAttackMontage->GetNumSections() - 1);
	PlayAnimMontage(MeleeFistAttackMontage, 1.f, FName(MontageSectionName + FString::FromInt(MontageSectionIndex)));
	CharState = EP9CharState::ATTACKING;
	/* OnPunchHandle should be fired next, after the anim notify is triggered. */
	
	Log(EP9LogLevel::INFO, MontageSectionName + FString::FromInt(MontageSectionIndex));
}

void AP9PunchCharacter::OnPunchHandle(USkeletalMeshComponent* MeshComp, const bool bStart)
{
	if (MeshComp != GetMesh()) return;
	Log(EP9LogLevel::INFO, bStart ? "Attack start" : "Attack end");
	/* Setting Fists collisions depending on the input condition. */
	LeftFistCollision->SetCollisionProfileName(bStart ? "Weapon" : "NoCollision");
	RightFistCollision->SetCollisionProfileName(bStart ? "Weapon" : "NoCollision");
	/* Set correct character state. */
	if (!bStart) CharState = EP9CharState::IDLE;
}

void AP9PunchCharacter::InitAnimNotifies()
{
	TArray<FAnimNotifyEvent> AnimNotifyEvents = MeleeFistAttackMontage->Notifies;
	for (const FAnimNotifyEvent& NotifyEvent : AnimNotifyEvents)
	{
		UP9NotifyWindowPunch* NotifyWindowPunch = Cast<UP9NotifyWindowPunch>(NotifyEvent.NotifyStateClass);
		if (NotifyWindowPunch)
		{
			NotifyWindowPunch->OnPunchStart.AddUObject<ThisClass, bool>(this, &ThisClass::OnPunchHandle, true);
			NotifyWindowPunch->OnPunchEnd.AddUObject<ThisClass, bool>(this, &ThisClass::OnPunchHandle, false);
		}
	}
}

void AP9PunchCharacter::Log(const EP9LogLevel Level, const FString& Message)
{
	Log(Level, Message, EP9LogOutput::ALL);
}

void AP9PunchCharacter::Log(const EP9LogLevel Level, const FString& Message, const EP9LogOutput LogOutput)
{
	/* Only print when screen is selected and GEngine object is available */
	if (GEngine &&(LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::SCREEN))
	{
		/* Default color */
		FColor LogColor;
		/* Flip the color based on the type */
		if (Level == EP9LogLevel::TRACE)        LogColor = FColor::Green;
		else if (Level == EP9LogLevel::DEBUG)   LogColor = FColor::Cyan;
		else if (Level == EP9LogLevel::INFO)    LogColor = FColor::White;
		else if (Level == EP9LogLevel::WARNING) LogColor = FColor::Yellow;
		else if (Level == EP9LogLevel::ERROR)   LogColor = FColor::Red;
		/* Print the message and leave it on screen */
		GEngine->AddOnScreenDebugMessage(-1, 5.f, LogColor, Message);
	}
	if (LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::LOG)
	{
		// flip the message type based on error level
		switch (Level)
		{
		case EP9LogLevel::TRACE:   UE_LOG(LogP9PunchCharacter, VeryVerbose, TEXT("%s"), *Message); break;
		case EP9LogLevel::DEBUG:   UE_LOG(LogP9PunchCharacter, Verbose, TEXT("%s"), *Message);     break;
		case EP9LogLevel::INFO:    UE_LOG(LogP9PunchCharacter, Log, TEXT("%s"), *Message);         break;
		case EP9LogLevel::WARNING: UE_LOG(LogP9PunchCharacter, Warning, TEXT("%s"), *Message);     break;
		case EP9LogLevel::ERROR:   UE_LOG(LogP9PunchCharacter, Error, TEXT("%s"), *Message);       break;
		default:                   UE_LOG(LogP9PunchCharacter, Log, TEXT("%s"), *Message);         break;
		}
	}
}

float AP9PunchCharacter::GetMovementDirectionAngle()
{
	if (FMath::IsNearlyZero(GetVelocity().Size2D())) return 0.f;
	const FVector Vector1 = GetActorForwardVector();
	const FVector Vector2 = GetVelocity().GetSafeNormal();
	const float Sign = FMath::Sign(Vector1.Cross(Vector2).Z);
	return Sign * FMath::RadiansToDegrees(FMath::Acos(Vector1.Dot(Vector2)));
}
