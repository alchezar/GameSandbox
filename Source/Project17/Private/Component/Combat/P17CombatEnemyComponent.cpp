// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatEnemyComponent.h"

void UP17CombatEnemyComponent::OnHitTargetActorCallback(AActor* HitActor)
{
	RETURN_IF(!HitActor || OverlappedActors.Contains(HitActor),)
	OverlappedActors.Add(HitActor);

	RETURN_IF(GetIsBlocked(),)
	SendGameplayEventToActor(HitActor);
}

bool UP17CombatEnemyComponent::GetIsBlocked()
{
	bool bValidBlock = false;
	const bool bPlayerBlocking = false;
	const bool bUnblockableAttack = true;

	if (bPlayerBlocking && !bUnblockableAttack)
	{
		// TODO::Check if the block is valid.
	}
	if (bValidBlock)
	{
		// TODO::Handle successful block.
	}

	return bValidBlock;
}
