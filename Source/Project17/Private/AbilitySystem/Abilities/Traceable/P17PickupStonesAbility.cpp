// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/Traceable/P17PickupStonesAbility.h"

#include "Project17.h"
#include "Component/UI/P17UIHeroComponent.h"
#include "Item/Pickup/P17StoneBase.h"

UP17PickupStonesAbility::UP17PickupStonesAbility()
{
	TraceDistance = 50.f;
	TraceBoxSize = FVector {200.f};
}

void UP17PickupStonesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ToggleUI(true);
}

void UP17PickupStonesAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	ToggleUI(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP17PickupStonesAbility::CollectStones()
{
	CollectedStones.Empty();
	for (const FHitResult& HitResult : BoxTrace())
	{
		AP17StoneBase* Stone = Cast<AP17StoneBase>(HitResult.GetActor());
		CONTINUE_IF(!Stone)

		CollectedStones.AddUnique(Stone);
	}

	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UP17PickupStonesAbility::ConsumeStones()
{
	RETURN_IF(CollectedStones.IsEmpty(),)

	for (AP17StoneBase* Stone : CollectedStones)
	{
		CONTINUE_IF(!Stone)
		Stone->Consume(GetProjectAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
	}
}

void UP17PickupStonesAbility::ToggleUI(const bool bOn)
{
	const UP17UIHeroComponent* HeroUIComponent = GetHeroUIComponentFromActorInfo();
	RETURN_IF(!HeroUIComponent,)

	HeroUIComponent->OnStoneInteracted.Broadcast(bOn);
}
