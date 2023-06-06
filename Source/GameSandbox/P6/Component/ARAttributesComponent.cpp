// Copyright (C) 2023, IKinder

#include "ARAttributesComponent.h"

UARAttributesComponent::UARAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = DefaultHealth;
}

void UARAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UARAttributesComponent::TryChangeHealth(const float Delta)
{
	Health += Delta;

	AROnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

