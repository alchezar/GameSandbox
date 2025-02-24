// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

UP16AbilitySystemComponent::UP16AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP16AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP16AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
