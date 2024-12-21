// Copyright (C) 2023, IKinder

#include "Pickup/STU_AmmoPickup.h"

#include "Component/STU_HealthComponent.h"
#include "Component/STU_WeaponComponent.h"

ASTU_AmmoPickup::ASTU_AmmoPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_AmmoPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_AmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTU_AmmoPickup::TryToGivePickup(APawn* PlayerPawn)
{
	const auto HealthComponent = Cast<USTU_HealthComponent>(PlayerPawn->GetComponentByClass(USTU_HealthComponent::StaticClass()));
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = Cast<USTU_WeaponComponent>(PlayerPawn->GetComponentByClass(USTU_WeaponComponent::StaticClass()));
	if (!WeaponComponent) return false;

	return WeaponComponent->TryToAddAmmo(WeaponType, Clips);
}
