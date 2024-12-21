// Copyright (C) 2023. IKinder

#include "Project01/Public/Character/PickupTrooper.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Project01/Public/Character/BatteryCollectorGameMode.h"
#include "Project01/Public/PickUp/PickupBase.h"
#include "Project01/Public/PickUp/PickUpBattery.h"

#pragma region Default

APickupTrooper::APickupTrooper()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(200.f);

	CurrentPowerLevel = BasePowerLevel;
}

void APickupTrooper::BeginPlay()
{
	Super::BeginPlay();

	CreateDynamicMaterials();
}

void APickupTrooper::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + (SpeedMultiplier * CurrentPowerLevel);
	UpdateDynamicMaterial();
}

#pragma endregion // Default

#pragma region Input

void APickupTrooper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
	{
		return;
	}
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(CollectAction, ETriggerEvent::Triggered, this, &ThisClass::BindCollectPickups);
	}
}

#pragma endregion // Input

#pragma region Pickup

void APickupTrooper::BindCollectPickups()
{
	TArray<AActor*> OverlapActors;
	float           CachedPowerLevel = 0.f;
	Sphere->GetOverlappingActors(OUT OverlapActors, APickUpBase::StaticClass());
	for (AActor* const OverlapActor : OverlapActors)
	{
		APickUpBase* PickupActor = Cast<APickUpBase>(OverlapActor);
		if (PickupActor && PickupActor->GetPickUpActive() && IsValid(PickupActor))
		{
			if (const APickUpBattery* BatteryActor = Cast<APickUpBattery>(PickupActor))
			{
				CachedPowerLevel += BatteryActor->GetChargeAmount();
			}
			PickupActor->OnPickupCollect();
			PickupActor->SetPickUpActive(false);
		}
	}
	if (CachedPowerLevel > 0.f)
	{
		UpdateCurrentPowerLevel(CachedPowerLevel);
	}
}

USphereComponent* APickupTrooper::GetSphereCollision() const
{
	return Sphere;
}

#pragma endregion // Pickup

#pragma region Power

float APickupTrooper::GetBasePowerLevel() const
{
	return BasePowerLevel;
}

float APickupTrooper::GetCurrentPowerLevel() const
{
	return CurrentPowerLevel;
}

void APickupTrooper::UpdateCurrentPowerLevel(const float Amount)
{
	CurrentPowerLevel += Amount;
}

#pragma endregion // Power

#pragma region Effect

void APickupTrooper::CreateDynamicMaterials()
{
	UMaterialInterface* MaterialToEdit1 = GetMesh()->GetMaterial(0);
	UMaterialInterface* MaterialToEdit2 = GetMesh()->GetMaterial(1);
	UMaterialInterface* MaterialToEdit3 = GetMesh()->GetMaterial(2);

	DynamicMaterial1 = UMaterialInstanceDynamic::Create(MaterialToEdit1, this);
	DynamicMaterial2 = UMaterialInstanceDynamic::Create(MaterialToEdit2, this);
	DynamicMaterial3 = UMaterialInstanceDynamic::Create(MaterialToEdit3, this);

	GetMesh()->SetMaterial(0, DynamicMaterial1);
	GetMesh()->SetMaterial(1, DynamicMaterial2);
	GetMesh()->SetMaterial(2, DynamicMaterial3);
}

void APickupTrooper::UpdateDynamicMaterial()
{
	const float PowerToWin = Cast<ABatteryCollectorGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetPowerToWin();
	check(PowerToWin);
	const float        Alpha        = 1 - GetCurrentPowerLevel() / PowerToWin;
	const FLinearColor DynamicColor = FMath::Lerp(StartColor, EndColor, FMath::Clamp(Alpha, 0.f, 1.f));

	DynamicMaterial1->SetVectorParameterValue("Color", DynamicColor);
	DynamicMaterial2->SetVectorParameterValue("Color", DynamicColor);
	DynamicMaterial3->SetVectorParameterValue("Color", DynamicColor);

	DynamicMaterial1->SetScalarParameterValue("Metallic", 0);
	DynamicMaterial2->SetScalarParameterValue("Metallic", 0);
	DynamicMaterial3->SetScalarParameterValue("Metallic", 0);
}

#pragma endregion // Effect
