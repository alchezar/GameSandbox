// Copyright © 2025, Ivan Kinder

#include "Player/P17CharacterHero.h"

#include "EnhancedInputSubsystems.h"
#include "Project17.h"
#include "Camera/CameraComponent.h"
#include "Component/Input/P17InputComponent.h"
#include "Data/P17DataAsset_InputConfig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Util/P17DebugHelper.h"
#include "Util/P17GameplayTags.h"

AP17CharacterHero::AP17CharacterHero()
{
	P17::Debug::Print("Working!");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmCameraBoomComponent");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = {0.f, 55.f, 65.f};
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCameraComponent");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	if (auto* CharMove = GetCharacterMovement())
	{
		CharMove->bOrientRotationToMovement = false;
		CharMove->RotationRate = {0.f, 500.f, 0.f};
		CharMove->MaxWalkSpeed = 400.f;
		CharMove->BrakingDecelerationWalking = 2000.f;
	}
}

void AP17CharacterHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const auto* PlayerController = GetController<APlayerController>();
	WARN_UNLESS(PlayerController)
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		WARN_RETURN_IF(!LocalPlayer,)
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		WARN_RETURN_IF(!Subsystem,)
		WARN_RETURN_IF(!InputConfig,)
		Subsystem->AddMappingContext(InputConfig->GetInputMappingContext(), 0);
	}

	UP17InputComponent* MyInputComponent = Cast<UP17InputComponent>(PlayerInputComponent);
	WARN_UNLESS(PlayerController)
	{
		MyInputComponent->BindAction(InputConfig->FindNativeInputAction(P17::Tags::Input_Move), ETriggerEvent::Started, this, &ThisClass::ToggleUseControllerRotation, true);
		MyInputComponent->BindAction(InputConfig->FindNativeInputAction(P17::Tags::Input_Move), ETriggerEvent::Completed, this, &ThisClass::ToggleUseControllerRotation, false);
		MyInputComponent->BindAction(InputConfig->FindNativeInputAction(P17::Tags::Input_Move), ETriggerEvent::Canceled, this, &ThisClass::ToggleUseControllerRotation, false);
		// I don't get it why we need custom input component. To have a worse interface?
		MyInputComponent->BindNativeInputFunction(InputConfig, P17::Tags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMoveCallback);
		MyInputComponent->BindNativeInputFunction(InputConfig, P17::Tags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::OnLookCallback);
	}
}

void AP17CharacterHero::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMeshComponent* MainMesh = GetMesh();
	WARN_RETURN_IF(!MainMesh,)
	for (const TObjectPtr<USceneComponent>& Child : MainMesh->GetAttachChildren())
	{
		auto* SkinnedChild = Cast<USkinnedMeshComponent>(Child);
		CONTINUE_IF(!SkinnedChild)
		SkinnedChild->SetLeaderPoseComponent(MainMesh);
	}
}

void AP17CharacterHero::BeginPlay()
{
	Super::BeginPlay();
}

void AP17CharacterHero::ToggleUseControllerRotation(const bool bStart)
{
	GetCharacterMovement()->bUseControllerDesiredRotation = bStart;
}

void AP17CharacterHero::OnMoveCallback(const FInputActionValue& InputActionValue)
{
	const auto MovementVector = InputActionValue.Get<FVector2D>();
	const auto MovementRotation = FRotator {0.f, Controller->GetControlRotation().Yaw, 0.f};

	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AP17CharacterHero::OnLookCallback(const FInputActionValue& InputActionValue)
{
	const auto LookAxisVector = InputActionValue.Get<FVector2D>();

	if (!FMath::IsNearlyZero(LookAxisVector.X))
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (!FMath::IsNearlyZero(LookAxisVector.Y))
	{
		// Clamp pitch rotation, to avoid camera extreme angle.
		const FVector UpVector = GetMesh()->GetUpVector();
		const FVector LookVector = GetControlRotation().Vector();
		const float DotProduct = UpVector | LookVector;
		RETURN_IF((DotProduct > 0.9 && LookAxisVector.Y < 0.f)
			|| (DotProduct < -0.9 && LookAxisVector.Y > 0.f),)

		AddControllerPitchInput(LookAxisVector.Y);
	}
}
