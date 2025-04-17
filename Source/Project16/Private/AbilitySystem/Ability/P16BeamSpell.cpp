// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16BeamSpell.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"

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

void UP16BeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	FHitResult HitResult;

	FVector Start = GetAvatarActorFromActorInfo()->GetActorLocation();
	if (const USkeletalMeshComponent* Weapon = IP16CombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo()))
	{
		Start = Weapon->GetSocketLocation(P16::Socket::Weapon);
	}
	const FQuat     Rotation = (BeamTargetLocation - Start).Rotation().Quaternion();
	constexpr float Radius   = 30.f;

	FCollisionQueryParams Params {};
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		BeamTargetLocation,
		Rotation,
		P16::CollisionChannel::Weapon,
		FCollisionShape::MakeSphere(Radius),
		Params);

	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor    = HitResult.GetActor();
	}
}

TArray<AActor*> UP16BeamSpell::StoreAdditionalTargets() const
{
	const UObject*        WorldContext  = GetAvatarActorFromActorInfo();
	const TArray<AActor*> IgnoredActors = {GetAvatarActorFromActorInfo(), MouseHitActor};
	constexpr float       Radius        = 850.f;

	TArray<AActor*> OverlappingActors = UP16AbilitySystemLibrary::GetLivePlayersWithinRadius(
		WorldContext,
		IgnoredActors,
		Radius,
		MouseHitLocation);
	const int32 NumTargets = FMath::Min(GetAbilityLevel(), MaxNumTargets);

	return UP16AbilitySystemLibrary::GetClosestTargets(NumTargets, MoveTemp(OverlappingActors), MouseHitLocation);
}
