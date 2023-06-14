// Copyright (C) 2023, IKinder

#include "ARAbilityComponent.h"
#include "P6/Ability/ARAbility.h"

UARAbilityComponent::UARAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UARAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Silver, DebugMsg);
}

void UARAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<UARAbility> ActionClass)
{
	if (!ActionClass) return;

	UARAbility* NewAbility = NewObject<UARAbility>(this, ActionClass);
	if (!NewAbility) return;

	Abilities.Add(NewAbility);
	if (NewAbility->bAutostart && NewAbility->CanStart(Instigator))
	{
		NewAbility->StartAbility(Instigator);
	}
}

void UARAbilityComponent::RemoveAbility(UARAbility* AbilityToRemove)
{
	if (!AbilityToRemove || AbilityToRemove->GetIsRunning()) return;	
	Abilities.Remove(AbilityToRemove);
}

bool UARAbilityComponent::StartAbilityByName(AActor* Instigator, const FName AbilityName)
{
	for (UARAbility* Ability : Abilities)
	{
		if (!Ability || Ability->AbilityName != AbilityName || !Ability->CanStart(Instigator)) continue;

		Ability->StartAbility(Instigator);
		return true;
	}
	return false;
}

bool UARAbilityComponent::StopAbilityByName(AActor* Instigator, const FName AbilityName)
{
	for (UARAbility* Ability : Abilities)
	{
		if (!Ability || Ability->AbilityName != AbilityName || !Ability->GetIsRunning()) continue;
		
		Ability->StopAbility(Instigator);
		return true;
	}
	return false;
}
