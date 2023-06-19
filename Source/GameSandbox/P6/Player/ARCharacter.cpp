// Copyright (C) 2023, IKinder

#include "ARCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P6/Ability/ARAbility.h"
#include "P6/Component/ARAbilityComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Component/ARInteractionComponent.h"
#include "P6/Game/ARGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogARCharacter, All, All);

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructComponents();
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(InteractionComp);
	check(AttributeComp);

	AddMappingContext();
	SetTeamColor();
	ExposeAbilities();
}

void AARCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARCharacter::ConstructComponents()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f);
	SpringArmComp->TargetArmLength = 250.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<UARInteractionComponent>("InteractionComponent");
	AttributeComp = CreateDefaultSubobject<UARAttributesComponent>("AttributesComponent");
	AbilityComp = CreateDefaultSubobject<UARAbilityComponent>("ActionComponent");
}

void AARCharacter::AddMappingContext() const
{
	const auto PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::Fire);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::PrimaryInteract);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::SprintStart);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::SprintStop);
	EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ThisClass::Parry);
	EnhancedInputComponent->BindAction(SaveGameAction, ETriggerEvent::Started, this, &ThisClass::SaveGame);
}

void AARCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	if (!Controller) return;

	const FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, MovementVector.X);
	AddMovementInput(ControlRot.Vector(), MovementVector.Y);
}

void AARCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AARCharacter::SetTeamColor() const
{
	UMaterialInstanceDynamic* DynamicBodyMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	UMaterialInstanceDynamic* DynamicBodyMaterial2 = GetMesh()->CreateAndSetMaterialInstanceDynamic(2);
	UMaterialInstanceDynamic* DynamicBodyMaterial3 = GetMesh()->CreateAndSetMaterialInstanceDynamic(3);

	if (!DynamicBodyMaterial1 || !DynamicBodyMaterial2 || !DynamicBodyMaterial3) return;

	DynamicBodyMaterial1->SetVectorParameterValue(TeamColorParameterName, TeamColor);
	DynamicBodyMaterial2->SetVectorParameterValue(TeamColorParameterName, TeamColor);
	DynamicBodyMaterial3->SetVectorParameterValue(TeamColorParameterName, TeamColor);
}

void AARCharacter::Fire()
{
	AbilityComp->StartAbilityByName(this, AttackTagName);
}

void AARCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract(this);
}

void AARCharacter::ExposeAbilities()
{
	if (!HasAuthority()) return; // ServerOnly
	
	for(const TSubclassOf<UARAbility> AbilityClass : AbilityClasses)
	{
		AbilityComp->AddAbility(this, AbilityClass);
	}
}

UARAttributesComponent* AARCharacter::GetAttributesComp() const
{
	return AttributeComp;
}

void AARCharacter::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (FMath::IsNearlyZero(NewHealth) && Delta < 0.f)
	{
		DisableInput(Cast<APlayerController>(Controller));

		// Ragdoll
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->MovementState.bCanJump = false;
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
	}
}

void AARCharacter::OnDeadHandle(AActor* DeadActor)
{}

void AARCharacter::HealSelf(float Amount)
{
	AttributeComp->TryChangeHealth(this, Amount);
}

void AARCharacter::SprintStart()
{
	AbilityComp->StartAbilityByName(this, SprintTagName);
}

void AARCharacter::SprintStop()
{
	AbilityComp->StopAbilityByName(this, SprintTagName);
}

void AARCharacter::Parry()
{
	AbilityComp->StartAbilityByName(this, ParryTagName);
}

void AARCharacter::SaveGame()
{
	if (AARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AARGameModeBase>())
	{
		GameMode->WriteSaveGame();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("World saved !")));
	}
}

UCameraComponent* AARCharacter::GetCameraComp() const
{
	return CameraComp;
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}