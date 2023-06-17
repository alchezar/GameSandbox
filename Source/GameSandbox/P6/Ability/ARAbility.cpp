// Copyright (C) 2023, IKinder

#include "ARAbility.h"
#include "Net/UnrealNetwork.h"
#include "P6/Component/ARAbilityComponent.h"
#include "P6/Util/ARFuncLibrary.h"

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
	// return Cast<UARAbilityComponent>(GetOuter());
	return AbilityComponent;
}

void UARAbility::StartAbility_Implementation(AActor* Instigator)
{
	UARFuncLibrary::LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *AbilityName.ToString()), FColor::Silver);
	
	UARAbilityComponent* AbilityComp = GetAbilityComponent();
	if (!AbilityComp) return;
	
	if (AbilityComp->ActiveGameplayTags.HasAll(GrantsTags)) return;

	AbilityComp->ActiveGameplayTags.AppendTags(GrantsTags);
	RepData = {true, Instigator};
}

void UARAbility::StopAbility_Implementation(AActor* Instigator)
{
	UARFuncLibrary::LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *AbilityName.ToString()), FColor::Silver);
	
	UARAbilityComponent* AbilityComp = GetAbilityComponent();
	if (!AbilityComp) return;
	
	if (!AbilityComp->ActiveGameplayTags.HasAll(GrantsTags)) return;
	
	AbilityComp->ActiveGameplayTags.RemoveTags(GrantsTags);
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