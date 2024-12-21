// Copyright (C) 2023, IKinder

#include "Pickup/STU_HealthPickup.h"

#include "Component/STU_HealthComponent.h"

ASTU_HealthPickup::ASTU_HealthPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_HealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_HealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTU_HealthPickup::TryToGivePickup(APawn* PlayerPawn)
{
	const auto HealthComponent = Cast<USTU_HealthComponent>(PlayerPawn->GetComponentByClass(USTU_HealthComponent::StaticClass()));
	if (!HealthComponent) return false;

	return HealthComponent->TryToAddHealth(HealAmount);
}
