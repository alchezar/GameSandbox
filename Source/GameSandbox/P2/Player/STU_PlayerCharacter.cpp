// Copyright (C) 2023, IKinder

#include "STU_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/STU_CharacterMovementComponent.h"
#include "Component/STU_WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASTU_PlayerCharacter::ASTU_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SocketOffset            = FVector(0.0, 60.0, 40.0);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTU_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCameraCollisionEndOverlap);

	MappingContext();
}

void ASTU_PlayerCharacter::OnDeathHandle()
{
	Super::OnDeathHandle();

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ASTU_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTU_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	auto* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;

	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &Super::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &ThisClass::StartRun);
	EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::StopRun);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchToggle);

	EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &USTU_WeaponComponent::StartFire);
	EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &USTU_WeaponComponent::StopFire);
	EnhancedInput->BindAction(NextWeaponAction, ETriggerEvent::Started, WeaponComponent, &USTU_WeaponComponent::NextWeapon);
	EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, WeaponComponent, &USTU_WeaponComponent::Reload);
	EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, WeaponComponent, &USTU_WeaponComponent::Aiming);
}

void ASTU_PlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (!Controller) return;

	FRotator      Rotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);
	const FVector Forward  = Rotation.Vector();
	AddMovementInput(Forward, MovementVector.Y);

	Rotation.Roll       = GetControlRotation().Roll;
	const FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(Right, MovementVector.X);
}

void ASTU_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);

	TurningInPlace();
}

void ASTU_PlayerCharacter::StartRun()
{
	bRunning = true;
}

void ASTU_PlayerCharacter::StopRun()
{
	bRunning = false;
}

bool ASTU_PlayerCharacter::GetIsRunning() const
{
	return bRunning;
}

void ASTU_PlayerCharacter::CrouchToggle()
{
	GetCharacterMovement()->IsCrouching() ? UnCrouch() : Crouch();
}

void ASTU_PlayerCharacter::TurningInPlace() const
{
	const double RotatorDelta = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, GetControlRotation().Yaw);
	const bool   bMove        = GetVelocity().Size() > 0.0;

	// Billet for adding an in-place turning animation blueprint depending on the left or right turn
	if (bMove || !bMove && (RotatorDelta > 100.0 || RotatorDelta < -100.0))
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement     = false;
	}
	else if (FMath::Abs(RotatorDelta) < 10.0)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement     = true;
	}
}

UCameraComponent* ASTU_PlayerCharacter::GetCameraComp() const
{
	return CameraComponent;
}

void ASTU_PlayerCharacter::MappingContext() const
{
	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ASTU_PlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ASTU_PlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ASTU_PlayerCharacter::CheckCameraOverlap()
{
	const bool bHideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(bHideMesh);

	TArray<USceneComponent*> AllChildren;
	GetMesh()->GetChildrenComponents(true, OUT AllChildren);
	for (const auto Child : AllChildren)
	{
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Child))
		{
			Component->SetOwnerNoSee(bHideMesh);
		}
	}
}
