// Copyright (C) 2023, IKinder

#include "ARAbility.h"
#include "P6/Component/ARAbilityComponent.h"

UWorld* UARAbility::GetWorld() const
{
	// return UObject::GetWorld();

	/* Outer set in ARAbilityComponent by NewObject<>() */
	const UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (!ActorComp) return nullptr;

	return ActorComp->GetWorld();
}

UARAbilityComponent* UARAbility::GetAbilityComponent() const
{
	return Cast<UARAbilityComponent>(GetOuter());
}

void UARAbility::StartAbility_Implementation(AActor* Instigator)
{
	GetAbilityComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	bRunning = true;
}

void UARAbility::StopAbility_Implementation(AActor* Instigator)
{
	GetAbilityComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	bRunning = false;
}

bool UARAbility::CanStart_Implementation(AActor* Instigator)
{
	if (bRunning) return false;
	return !GetAbilityComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

bool UARAbility::GetIsRunning() const
{
	return bRunning;
}
