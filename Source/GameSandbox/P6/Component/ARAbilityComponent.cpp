// Copyright (C) 2023, IKinder

#include "ARAbilityComponent.h"
#include "P6/Ability/ARAbility.h"

UARAbilityComponent::UARAbilityComponent()
{
	// PrimaryComponentTick.bCanEverTick = true;
}

void UARAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

// void UARAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// }

void UARAbilityComponent::AddAbility(TSubclassOf<UARAbility> ActionClass)
{
	if (!ActionClass) return;

	UARAbility* NewAction = NewObject<UARAbility>(this, ActionClass);
	if (!NewAction) return;

	Abilities.Add(NewAction);
}

bool UARAbilityComponent::StartAbilityByName(AActor* Instigator, FName ActionName)
{
	for (UARAbility* Action : Abilities)
	{
		if (!Action || Action->ActionName != ActionName) return false;

		Action->StartAction(Instigator);
		return true;
	}
	return false;
}

bool UARAbilityComponent::StopAbilityByName(AActor* Instigator, FName ActionName)
{
	for (UARAbility* Action : Abilities)
	{
		if (!Action || Action->ActionName != ActionName) return false;

		Action->StopAction(Instigator);
		return true;
	}
	return false;
}
