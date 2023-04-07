// Copyright (C) 2023, IKinder

#include "STU_BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameSandbox/P2/Component/STU_CharacterMovementComponent.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "GameSandbox/P2/Weapon/STU_BaseWeapon.h"

ASTU_BaseCharacter::ASTU_BaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	SetupComponent();
}

void ASTU_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	MappingContext();
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChangedHandle(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ASTU_BaseCharacter::OnDeathHandle);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTU_BaseCharacter::OnHealthChangedHandle);
	LandedDelegate.AddDynamic(this, &ASTU_BaseCharacter::LandedHandle);

	SpawnWeapon();
}

void ASTU_BaseCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Component

void ASTU_BaseCharacter::SetupComponent()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent     = CreateDefaultSubobject<USTU_HealthComponent>("HealthComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

UCameraComponent* ASTU_BaseCharacter::GetCameraComp() const
{
	return CameraComponent;
}

#pragma endregion // Component

#pragma region Input

void ASTU_BaseCharacter::MappingContext() const
{
	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ASTU_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	auto* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;

	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &Super::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &ThisClass::StartRun);
	EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::StopRun);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchToggle);
}

void ASTU_BaseCharacter::Move(const FInputActionValue& Value)
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

void ASTU_BaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ASTU_BaseCharacter::StartRun()
{
	bRunning = true;
}

void ASTU_BaseCharacter::StopRun()
{
	bRunning = false;
}

bool ASTU_BaseCharacter::GetIsRunning() const
{
	return bRunning;
}

void ASTU_BaseCharacter::CrouchToggle()
{
	GetCharacterMovement()->IsCrouching() ? UnCrouch() : Crouch();
}

float ASTU_BaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween   = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct   = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

#pragma endregion // Input

#pragma region Health

void ASTU_BaseCharacter::LandedHandle(const FHitResult& Hit)
{
	const double FallVelocity = -GetVelocity().Z;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%f"), FallVelocity));
	if (FallVelocity < LandedDamageVelocity.X) return;

	const double FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocity);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTU_BaseCharacter::OnHealthChangedHandle(const float Health)
{
	HealthTextComponent->SetText(FText::AsNumber(static_cast<int>(Health)));
}

void ASTU_BaseCharacter::OnDeathHandle()
{
	const int32 RandomElement = FMath::RandRange(0, DeathAnimations.Num() - 1);
	PlayAnimMontage(DeathAnimations[RandomElement]);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}
#pragma endregion // Health

#pragma region Weapon

void ASTU_BaseCharacter::SpawnWeapon()
{
	if(!GetWorld()) return;
	ASTU_BaseWeapon* Weapon = GetWorld()->SpawnActor<ASTU_BaseWeapon>(WeaponClass);
	if (!Weapon) return;	
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(GetMesh(), AttachmentRules, "HandWeaponSocket");	
}

#pragma endregion // Weapon