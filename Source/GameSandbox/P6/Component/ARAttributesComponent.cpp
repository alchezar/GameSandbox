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
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	AROnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return ActualDelta != 0.f;
}

bool UARAttributesComponent::GetIsAlive() const
{
	return Health > 0.f;
}

bool UARAttributesComponent::GetIsHealthMax() const
{
	return Health == HealthMax;
}

float UARAttributesComponent::GetHealthMax() const
{
	return HealthMax;
}
