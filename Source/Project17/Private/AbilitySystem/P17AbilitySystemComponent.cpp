// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P17AbilitySystemComponent.h"

UP17AbilitySystemComponent::UP17AbilitySystemComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UP17AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
