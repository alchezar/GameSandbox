// Copyright (C) 2023, IKinder

#include "ARCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Component/ARInteractionComponent.h"
#include "P6/UI/ARCrosshairWidget.h"
#include "P6/Weapon/ARProjectileMagic.h"

AARCharacter::AARCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructComponents();
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
	AttributeComp->AROnDead.AddDynamic(this, &ThisClass::OnDeadHandle);
}

void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(InteractionComp);
	check(AttributeComp);

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
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	const FVector Start = CameraComp->GetComponentLocation();
	const FVector End = Start + GetControlRotation().Vector() * FVector(10000.f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	// FTimerDelegate AttackDelegate;
	// AttackDelegate.BindLambda([=]()
	// {
	// 	SpawnProjectile(bHomingProjectile, ProjectileClass, HitResult);
	// });
	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [=]()
	{
		this->SpawnProjectile(bHomingProjectile, ProjectileClass, HitResult);
	}, 1.f, false, 0.2f);
}

void AARCharacter::SpawnProjectile(const bool bHoming, const TSubclassOf<AActor> ClassToSpawn, const FHitResult& HitResult)
{
	const FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (HitResult.bBlockingHit && bHoming)
	{
		ATargetPoint* Target = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), HitResult.ImpactPoint, FRotator::ZeroRotator);
		Target->SetLifeSpan(5.f);

		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ClassToSpawn, FTransform(GetActorRotation(), HandLocation), SpawnParams);
		if (!SpawnedProjectile) return;
		AARProjectileMagic* SpawnedHomingProjectile = Cast<AARProjectileMagic>(SpawnedProjectile);
		if (!SpawnedHomingProjectile) return;

		SpawnedHomingProjectile->AddActorToIgnore(this);
		SpawnedHomingProjectile->SetTarget(Target);
		return;
	}

	const FVector TargetLocation = HitResult.bBlockingHit
		? HitResult.ImpactPoint
		: CameraComp->GetComponentLocation() + GetControlRotation().Vector() * FVector(10000.f);

	const FRotator CorrectRotation = FRotationMatrix::MakeFromX(TargetLocation - HandLocation).Rotator();
	const FTransform SpawnTransform = FTransform(CorrectRotation, HandLocation, FVector(1.f));

	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);
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
	
	if (const auto CrosshairWidget = Cast<UARCrosshairWidget>(HUD))
	{
		CrosshairWidget->BindHealthToAliveBody(this);
	}
}

void AARCharacter::RemoveWidget()
{
	if (!HUD) return;
	HUD->RemoveFromParent();
}

UARAttributesComponent* AARCharacter::GetAttributesComp() const
{
	return AttributeComp;
}

void AARCharacter::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (FMath::IsNearlyZero(NewHealth) && Delta < 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (!PlayerController) return;

		DisableInput(PlayerController);

		// Ragdoll
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->MovementState.bCanJump = false;
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
	}
}

void AARCharacter::OnDeadHandle(AActor* DeadActor)
{
	RemoveWidget();
}

void AARCharacter::HealSelf(float Amount)
{
	AttributeComp->TryChangeHealth(this, Amount);
}