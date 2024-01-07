// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"

UP13CharacterAttributesComponent::UP13CharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13CharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP13CharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP13CharacterAttributesComponent::ReceiveDamage(const float Damage)
{
	Super::ReceiveDamage(Damage);

	
}

