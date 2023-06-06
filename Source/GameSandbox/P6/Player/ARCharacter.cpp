// Copyright (C) 2023, IKinder

#include "ARCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P6/Component/ARInteractionComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Weapon/ARMagicProjectile.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructComponents();	
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(InteractionComp);
	
	AddMappingContext();
	SetTeamColor();
	AddWidget();
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
	FHitResult HitResult;
	const FVector Start = CameraComp->GetComponentLocation();
	const FVector End = Start + GetControlRotation().Vector() * 10000.f;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (!HitResult.bBlockingHit) return;
	
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	ATargetPoint* Target = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), HitResult.ImpactPoint, FRotator::ZeroRotator);
	Target->SetLifeSpan(5.f);

	FTimerDelegate AttackDelegate;
	AttackDelegate.BindLambda([=]()
	{
		const FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		// const FRotator CorrectRotation = FRotationMatrix::MakeFromX(HitResult.ImpactPoint - HandLocation).Rotator();		
		// const FTransform SpawnTransform = FTransform(CorrectRotation, HandLocation, FVector(1.f));
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, FTransform(GetActorRotation(), HandLocation), SpawnParams);
		// AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
		if (!SpawnedActor) return;
		AARMagicProjectile* SpawnedMagicProjectile = Cast<AARMagicProjectile>(SpawnedActor);
		if (!SpawnedMagicProjectile) return;

		SpawnedMagicProjectile->AddActorToIgnore(this);
		SpawnedMagicProjectile->SetTarget(Target);
	});
	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, AttackDelegate, 1.f, false, 0.2f);	
}

void AARCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void AARCharacter::AddWidget()
{
	if (!HUDClass) return;
	
	HUD = CreateWidget(GetWorld(), HUDClass);
	if (!HUD) return;

	HUD->AddToViewport();
}

UARAttributesComponent* AARCharacter::GetAttributesComp() const
{
	return AttributeComp;
}
