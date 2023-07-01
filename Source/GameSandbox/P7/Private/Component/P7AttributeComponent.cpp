// Copyright (C) 2023, IKinder

#include "P7/Public/Component/P7AttributeComponent.h"

UP7AttributeComponent::UP7AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP7AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

