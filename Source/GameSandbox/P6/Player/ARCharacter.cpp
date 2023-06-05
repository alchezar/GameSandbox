// Copyright (C) 2023, IKinder

#include "ARCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P6/Component/ARInteractionComponent.h"
#include "P6/Weapon/ARMagicProjectile.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructComponents();	
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(InteractionComponent);
	
	AddMappingContext();
	SetTeamColor();
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
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComponent = CreateDefaultSubobject<UARInteractionComponent>("InteractionComponent");
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

void AARCharacter::Fire()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	FTimerDelegate AttackDelegate;
	AttackDelegate.BindLambda([&]()
	{
		const FTransform SpawnTransform = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation(HandSocketName), FVector(1.f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
		if (!SpawnedActor) return;
		AARMagicProjectile* SpawnedMagicProjectile = Cast<AARMagicProjectile>(SpawnedActor);
		if (!SpawnedMagicProjectile) return;

		SpawnedMagicProjectile->AddActorToIgnore(this);
	});
	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, AttackDelegate, 1.f, false, 0.2f);	
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

void AARCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}
