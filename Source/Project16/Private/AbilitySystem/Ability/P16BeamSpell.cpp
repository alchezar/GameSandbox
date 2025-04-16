// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16BeamSpell.h"
#include "Project16.h"
#include "GameFramework/Character.h"

FString UP16BeamSpell::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = DamageInfo.Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost     = GetManaCost(CurrentLevel);
	const float CooldownTime = GetCooldownTime(CurrentLevel);

	return Super::GetDescription(CurrentLevel);
}

FString UP16BeamSpell::GetDescriptionNextLevel(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 NextLevel    = CurrentLevel + 1;
	const int32 DamageValue  = DamageInfo.Damage.GetValueAtLevel(NextLevel);
	const float ManaCost     = GetManaCost(NextLevel);
	const float CooldownCost = GetCooldownTime(NextLevel);

	return Super::GetDescriptionNextLevel(CurrentLevel);
}

void UP16BeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	MouseHitLocation = HitResult.ImpactPoint;
	MouseHitActor    = HitResult.GetActor();
}

void UP16BeamSpell::StoreOwnerVariables()
{
	EARLY_RETURN_IF(!CurrentActorInfo)

	OwnerController = CurrentActorInfo->PlayerController.Get();
	OwnerCharacter  = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
}
