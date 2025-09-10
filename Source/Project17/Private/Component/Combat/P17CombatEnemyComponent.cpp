// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatEnemyComponent.h"

#include "Util/P17FunctionLibrary.h"

void UP17CombatEnemyComponent::OnHitTargetActorCallback(AActor* HitActor)
{
	RETURN_IF(!HitActor || OverlappedActors.Contains(HitActor),)

	OverlappedActors.Add(HitActor);
	RETURN_IF(GetIsBlocked(HitActor),)

	SendGameplayEventToActor(HitActor, GetOwningPawn());
}

bool UP17CombatEnemyComponent::GetIsBlocked(AActor* HitActor) const
{
	const bool bPlayerBlocking = UP17FunctionLibrary::NativeGetActorHasTag(HitActor, P17::Tags::Player_Status_Blocking);
	const bool bUnblockableAttack = UP17FunctionLibrary::NativeGetActorHasTag(GetOwningPawn(), P17::Tags::Enemy_Status_Unblockable);

	const bool bValidBlock = UP17FunctionLibrary::IsValidBlock(GetOwner(), HitActor)
		&& bPlayerBlocking
		&& !bUnblockableAttack;

	if (bValidBlock)
	{
		SendGameplayEventToActor(HitActor, HitActor, P17::Tags::Player_Event_Hit_SuccessfulBlock);
	}

	return bValidBlock;
}
