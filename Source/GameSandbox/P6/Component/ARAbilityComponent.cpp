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

void UARAbilityComponent::AddAbility(TSubclassOf<UARAbility> ActionClass)
{
	if (!ActionClass) return;

	UARAbility* NewAction = NewObject<UARAbility>(this, ActionClass);
	if (!NewAction) return;

	Abilities.Add(NewAction);
}

bool UARAbilityComponent::StartAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (UARAbility* Ability : Abilities)
	{
		if (!Ability || Ability->AbilityName != AbilityName || !Ability->CanStart(Instigator)) continue;

		Ability->StartAbility(Instigator);
		return true;
	}
	return false;
}

bool UARAbilityComponent::StopAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (UARAbility* Ability : Abilities)
	{
		if (!Ability || Ability->AbilityName != AbilityName || !Ability->GetIsRunning()) continue;
		
		Ability->StopAbility(Instigator);
		return true;
	}
	return false;
}
