// Copyright (C) 2023, IKinder

#include "ARAbility.h"
#include "Net/UnrealNetwork.h"
#include "P6/Component/ARAbilityComponent.h"

UWorld* UARAbility::GetWorld() const
{
	/* Outer set in ARAbilityComponent by NewObject<>() */
	const AActor* ActorComp = Cast<AActor>(GetOuter());
	if (!ActorComp) return nullptr;

	return ActorComp->GetWorld();
}

void UARAbility::Initialize(UARAbilityComponent* NewAbilityComp)
{
	AbilityComponent = NewAbilityComp;
}

UARAbilityComponent* UARAbility::GetAbilityComponent() const
{
	return AbilityComponent;
}

void UARAbility::StartAbility_Implementation(AActor* Instigator)
{
	if (!AbilityComponent) return;
	
	AbilityComponent->HandleActionStart(GrantsTags, this);
	RepData = {true, Instigator};
}

void UARAbility::StopAbility_Implementation(AActor* Instigator)
{
	if (!AbilityComponent) return;

	AbilityComponent->HandleActionStop(GrantsTags, this);
	RepData = {false, Instigator};
}

bool UARAbility::CanStart_Implementation(AActor* Instigator)
{
	return !RepData.bRunning && GetAbilityComponent() && !GetAbilityComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

bool UARAbility::GetIsRunning() const
{
	return RepData.bRunning;
}

void UARAbility::OnRep_RepData()
{
	RepData.bRunning ? StartAbility(RepData.Instigator) : StopAbility(RepData.Instigator);
}

void UARAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, RepData);
	DOREPLIFETIME(ThisClass, AbilityComponent);
}

bool UARAbility::IsSupportedForNetworking() const
{
	return true;
}

UTexture2D* UARAbility::GetIcon() const
{
	return  Icon.Get();
}
