// Copyright (C) 2023, IKinder

#include "ARAbilityComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "P6/Ability/ARAbility.h"

UARAbilityComponent::UARAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UARAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UARAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<UARAbility> ActionClass)
{
	if (!ActionClass || !GetOwner()->HasAuthority()) return;

	UARAbility* NewAbility = NewObject<UARAbility>(GetOwner(), ActionClass);
	if (!NewAbility) return;

	NewAbility->Initialize(this);
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

		if (!GetOwner()->HasAuthority())
		{
			ServerStartAbility(Instigator, AbilityName);
		}
		
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

		if (!GetOwner()->HasAuthority())
		{
			ServerStopAbility(Instigator, AbilityName);
		}
		
		Ability->StopAbility(Instigator);
		return true;
	}
	return false;
}

void UARAbilityComponent::ServerStartAbility_Implementation(AActor* Instigator, const FName AbilityName)
{
	StartAbilityByName(Instigator, AbilityName);
}

void UARAbilityComponent::ServerStopAbility_Implementation(AActor* Instigator, const FName AbilityName)
{
	StopAbilityByName(Instigator, AbilityName);
}

void UARAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Abilities);
}

bool UARAbilityComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARAbility* Ability : Abilities)
	{
		if (!Ability) continue;

		bWroteSomething |= Channel->ReplicateSubobject(Ability, *Bunch, *RepFlags);
	}
	return bWroteSomething;
}

void UARAbilityComponent::HandleActionStart(const FGameplayTagContainer& GrantsTags, UARAbility* Ability)
{
	if (ActiveGameplayTags.HasAll(GrantsTags)) return;
	
	ActiveGameplayTags.AppendTags(GrantsTags);
	OnAbilityStarted.Broadcast(this, Ability);
}

void UARAbilityComponent::HandleActionStop(const FGameplayTagContainer& GrantsTags, UARAbility* Ability)
{
	if (!ActiveGameplayTags.HasAll(GrantsTags)) return;
	
	ActiveGameplayTags.RemoveTags(GrantsTags);
	OnAbilityStopped.Broadcast(this, Ability);
}

TArray<UARAbility*> UARAbilityComponent::GetAbilities() const
{
	return Abilities;
}
