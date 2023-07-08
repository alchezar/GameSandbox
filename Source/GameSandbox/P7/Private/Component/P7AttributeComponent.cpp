// Copyright (C) 2023, IKinder

#include "P7/Public/Component/P7AttributeComponent.h"

UP7AttributeComponent::UP7AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetHealth();
}

void UP7AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP7AttributeComponent::ReceiveDamage(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnReceiveDamage.Broadcast(CurrentHealth / MaxHealth);
}

void UP7AttributeComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
}
