// Copyright © 2024, Ivan Kinder

#include "Player/P14Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/P14InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AP14Character::AP14Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = {0.f, 540.f, 0.f};
		Movement->JumpZVelocity             = 600.f;
		Movement->AirControl                = 0.2f;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength         = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UP14InventoryComponent>("InventoryComponent");
}

void AP14Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (const TObjectPtr<USceneComponent>& MeshChild : GetMesh()->GetAttachChildren())
	{
		if (USkinnedMeshComponent* SkeletalChild = Cast<USkinnedMeshComponent>(MeshChild))
		{
			SkeletalChild->SetVisibility(ClothType == EP14ClothType::Skeletal);

			// Use one Anim Blueprint for all skeletal meshes.
			SkeletalChild->SetLeaderPoseComponent(GetMesh());
		}
	}
}

void AP14Character::BeginPlay()
{
	Super::BeginPlay();

	check(HealthData.MaxHealth > 0.f)
	UpdateHealth(HealthData.MaxHealth);
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnAnyDamageReceivedCallback);

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

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}
}

void AP14Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindActionValue(MoveAction);
		Input->BindActionValue(LookAction);
		Input->BindActionValue(JumpAction);

		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed | ETriggerEvent::Canceled, this, &Super::StopJumping);
	}
}

void AP14Character::MoveInput(const FInputActionValue& InputValue)
{
	check(Controller)

	const FVector2D InputVector      = InputValue.Get<FVector2D>();
	const FVector   ForwardDirection = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::ForwardVector);
	const FVector   RightDirection   = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AP14Character::LookInput(const FInputActionValue& InputValue)
{
	const FVector2D InputVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}

void AP14Character::OnAnyDamageReceivedCallback(AActor* DamagedActor, const float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	const auto IsDead = [this]() -> bool
	{
		return Health <= 0.f;
	};

	if (Damage <= 0.f || IsDead())
	{
		return;
	}

	UpdateHealth(-Damage);

	if (IsDead())
	{
		OnDeath();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(HealthTimer, this, &ThisClass::OnHealing, HealthData.HealRate, true);
	}
}

void AP14Character::UpdateHealth(const float HealthDelta)
{
	Health = FMath::Clamp(Health + HealthDelta, 0.f, HealthData.MaxHealth);
	OnHealthChanged.Broadcast();
}

void AP14Character::OnHealing()
{
	UpdateHealth(HealthData.HealModifier);

	if (Health >= HealthData.MaxHealth)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
		Health = HealthData.MaxHealth;
	}
}

void AP14Character::OnDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthTimer);

	check(GetCharacterMovement())
	check(GetCapsuleComponent())
	check(GetMesh())

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	if (AController* Control = GetController())
	{
		Control->ChangeState(NAME_Spectating);
	}
	DetachFromControllerPendingDestroy();
	SetLifeSpan(HealthData.LifeSpan);
}
