// Copyright © 2025, Ivan Kinder

#include "Player/P15Character.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Project15.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP15Character::AP15Character()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmCameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent.Get());

	PlayerEye = CreateDefaultSubobject<UCameraComponent>("PlayerEyeCameraComponent");
	PlayerEye->SetupAttachment(CameraBoom.Get());

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed             = P15::WalkSpeed;
		Movement->NavAgentProps.bCanCrouch = true;
		Movement->SetCrouchedHalfHeight(66.f);

		MaxCrouchOffset = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - GetCharacterMovement()->GetCrouchedHalfHeight();
	}
}

void AP15Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Use one Anim Blueprint for all skeletal meshes.
	for (const TObjectPtr<USceneComponent>& MeshChild : GetMesh()->GetAttachChildren())
	{
		USkinnedMeshComponent* SkeletalChild = Cast<USkinnedMeshComponent>(MeshChild);
		if (!SkeletalChild)
		{
			continue;
		}

		SkeletalChild->SetLeaderPoseComponent(GetMesh());
	}
}

void AP15Character::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	AcquireAbility(MeleeAbility);
}

void AP15Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeWalkSpeedSmoothly(DeltaTime);
	UpdateCameraBoomOffsetSmoothly(DeltaTime);
}

void AP15Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		Input->BindAction(LookAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		Input->BindAction(JumpAction.Get(), ETriggerEvent::Started, this, &Super::Jump);
		Input->BindAction(JumpAction.Get(), ETriggerEvent::Completed, this, &Super::StopJumping);
		Input->BindAction(RunAction.Get(), ETriggerEvent::Started, this, &ThisClass::RunInput, true);
		Input->BindAction(RunAction.Get(), ETriggerEvent::Completed, this, &ThisClass::RunInput, false);
		Input->BindAction(CrouchAction.Get(), ETriggerEvent::Started, this, &ThisClass::CrouchInput);
		Input->BindAction(AttackAction.Get(), ETriggerEvent::Started, this, &ThisClass::AttackInput);
	}
}

UAbilitySystemComponent* AP15Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComp.Get();
}

void AP15Character::MoveInput(const FInputActionValue& InputValue)
{
	if (!Controller)
	{
		return;
	}

	const FVector ForwardDirection = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::ForwardVector);
	const FVector RightDirection   = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::RightVector);

	const FVector2D InputVector = InputValue.Get<FVector2D>();
	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AP15Character::LookInput(const FInputActionValue& InputValue)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D InputVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		const bool bMoving = Movement->Velocity.SizeSquared() > P15::MinimalSpeed;
		const bool bInAir  = Movement->IsFalling();

		Movement->bUseControllerDesiredRotation = bMoving && !bInAir;
	}
}

void AP15Character::RunInput(const bool bRun)
{
	SpeedChangeData.bActive   = true;
	SpeedChangeData.bPositive = bRun;
	SpeedChangeData.Alpha     = 0.f;
}

void AP15Character::CrouchInput()
{
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->SocketOffset.Z   = bIsCrouched ? -MaxCrouchOffset : MaxCrouchOffset;
	bIsCrouched ? UnCrouch() : Crouch();

	CameraOffsetChangeData.bActive   = true;
	CameraOffsetChangeData.bPositive = !bIsCrouched;
	CameraOffsetChangeData.Alpha     = 0.f;
}

void AP15Character::AttackInput()
{
	AbilitySystemComp->TryActivateAbilityByClass(MeleeAbility);
}

void AP15Character::AddDefaultMappingContext() const
{
	const APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController)
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!Subsystem)
	{
		return;
	}

	Subsystem->AddMappingContext(InputContext, 0);
}

void AP15Character::ChangeWalkSpeedSmoothly(const float DeltaTime)
{
	if (!SpeedChangeData.bActive)
	{
		return;
	}

	float&      CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	const float StartSpeed   = SpeedChangeData.bPositive ? P15::WalkSpeed : P15::RunSpeed;
	const float TargetSpeed  = SpeedChangeData.bPositive ? P15::RunSpeed : P15::WalkSpeed;

	float& Alpha = SpeedChangeData.Alpha;
	if (Alpha >= 1.f)
	{
		SpeedChangeData.bActive = false;
		CurrentSpeed            = TargetSpeed;
		return;
	}

	CurrentSpeed = FMath::InterpSinOut(StartSpeed, TargetSpeed, Alpha);
	Alpha += DeltaTime / CameraOffsetChangeData.Time;
}

void AP15Character::UpdateCameraBoomOffsetSmoothly(const float DeltaTime)
{
	if (!CameraOffsetChangeData.bActive)
	{
		return;
	}

	double&          CurrentOffset = CameraBoom->SocketOffset.Z;
	const double     StartOffset   = CameraOffsetChangeData.bPositive ? MaxCrouchOffset : -MaxCrouchOffset;
	constexpr double TargetOffset  = 0.f;

	float& Alpha = CameraOffsetChangeData.Alpha;
	if (Alpha >= 1.f)
	{
		CameraBoom->bEnableCameraLag   = true;
		CameraOffsetChangeData.bActive = false;
		CurrentOffset                  = TargetOffset;
		return;
	}

	CurrentOffset = FMath::InterpSinOut(StartOffset, TargetOffset, Alpha);
	Alpha += DeltaTime / CameraOffsetChangeData.Time;
}

void AP15Character::AcquireAbility(const TSubclassOf<UGameplayAbility>& AbilityToAcquire)
{
	if (!AbilitySystemComp)
	{
		return;
	}

	if (HasAuthority() && AbilityToAcquire)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec{AbilityToAcquire});
	}
	AbilitySystemComp->InitAbilityActorInfo(this, this);
}
