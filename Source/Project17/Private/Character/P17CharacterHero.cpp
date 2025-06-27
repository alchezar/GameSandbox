// Copyright © 2025, Ivan Kinder

#include "Public/Character/P17CharacterHero.h"

#include "EnhancedInputSubsystems.h"
#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/Combat/P17CombatHeroComponent.h"
#include "Component/Input/P17InputComponent.h"
#include "Data/P17Data_InputConfig.h"
#include "Data/P17Data_StartupBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Util/P17GameplayTags.h"

AP17CharacterHero::AP17CharacterHero()
{
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

	CombatComponent = CreateDefaultSubobject<UP17CombatHeroComponent>("HeroCombatComponent");
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
		Subsystem->AddMappingContext(InputConfig->DefaultContext, 0);
	}

	UP17InputComponent* MyInputComponent = Cast<UP17InputComponent>(PlayerInputComponent);
	WARN_UNLESS(PlayerController)
	{
		MyInputComponent->BindNativeInputAction(InputConfig, P17::Tags::Input_Move, this, &ThisClass::ToggleUseControllerRotation);
		MyInputComponent->BindNativeInputAction(InputConfig, P17::Tags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMoveCallback);
		MyInputComponent->BindNativeInputAction(InputConfig, P17::Tags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::OnLookCallback);
		MyInputComponent->BindNativeInputAction(InputConfig, P17::Tags::Input_Jump, ETriggerEvent::Started, this, &Super::Jump);

		MyInputComponent->BindAbilityInputAction(InputConfig, this, &ThisClass::OnAbilityInputPressedCallback, &ThisClass::OnAbilityInputReleasedCallback);
	}
}

void AP17CharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	WARN_RETURN_IF(StartupData.IsNull(),)
	UP17Data_StartupBase* LoadedData = StartupData.LoadSynchronous();
	WARN_RETURN_IF(!LoadedData,)
	LoadedData->GiveToAbilitySystemComponent(GetProjectAbilitySystemComponent());
}

UP17CombatPawnComponent* AP17CharacterHero::GetCombatComponent() const
{
	return GetHeroComponent();
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

void AP17CharacterHero::OnAbilityInputPressedCallback(const FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AP17CharacterHero::OnAbilityInputReleasedCallback(const FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
