// Copyright © 2025, Ivan Kinder

#include "Component/P16InputComponent.h"

UP16InputComponent::UP16InputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP16InputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP16InputComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
