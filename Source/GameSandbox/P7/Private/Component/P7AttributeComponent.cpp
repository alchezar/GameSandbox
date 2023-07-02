// Copyright (C) 2023, IKinder

#include "P7/Public/Component/P7AttributeComponent.h"

UP7AttributeComponent::UP7AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void UP7AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP7AttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

