// Copyright (C) 2023, IKinder

#include "ARAttributesComponent.h"

UARAttributesComponent::UARAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = HealthMax;
}

void UARAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UARAttributesComponent::TryChangeHealth(const float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	AROnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

bool UARAttributesComponent::GetIsAlive() const
{
	return Health > 0.f;
}
