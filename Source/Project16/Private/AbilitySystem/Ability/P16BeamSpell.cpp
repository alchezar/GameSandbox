// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16BeamSpell.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"

#define LISTEN_TO_TARGET_DEATH(Target, Callback)                                   \
if (const TScriptInterface<IP16CombatInterface> CombatInterface = Target)          \
{                                                                                  \
	FP16OnDeathSignature& OnDeathDelegate = CombatInterface->GetOnDeathDelegate(); \
	if (!OnDeathDelegate.IsAlreadyBound(this, &ThisClass::Callback))               \
	{                                                                              \
		OnDeathDelegate.AddDynamic(this, &ThisClass::Callback);                    \
	}                                                                              \
}

FString UP16BeamSpell::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = GetDamageAtLevel(CurrentLevel);
	const float ManaCost     = GetManaCost(CurrentLevel);
	const float CooldownTime = GetCooldownTime(CurrentLevel);

	FString Description = FRichString {}
		// Title.
		.Add(Title, "ELECTROCUTE")
		.Gap(Double)
		// Details.
		.Add(Small, "Level: ")
		.Num(Level, CurrentLevel)
		.Gap(Single)
		.Add(Small, "Mana cost: ")
		.Num(Mana, ManaCost)
		.Gap(Single)
		.Add(Small, "Cooldown: ")
		.Num(Cooldown, CooldownTime)
		.Gap(Double)
		// Description.
		.Add(Default, "Emits ")
		.Get();

	if (CurrentLevel == 1)
	{
		Description += FRichString {}
			.Add(Default, "1 beam ")
			.Get();
	}
	else
	{
		Description += FRichString {}
			.Num(Default, FMath::Min(CurrentLevel, MaxNumTargets))
			.Add(Default, " beams ")
			.Get();
	}

	Description += FRichString {}
		.Add(Default, "of lightning, connecting with the target, repeatedly causing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " lightning damage with a chance to stun.")
		.Get();

	return Description;
}

FString UP16BeamSpell::GetDescriptionNextLevel(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 NextLevel    = CurrentLevel + 1;
	const int32 DamageValue  = GetDamageAtLevel(NextLevel);
	const float ManaCost     = GetManaCost(NextLevel);
	const float CooldownCost = GetCooldownTime(NextLevel);

	return FRichString {}
		// Title.
		.Add(Title, "NEXT LEVEL")
		.Gap(Double)
		// Details.
		.Add(Small, "Level: ")
		.Num(Level, NextLevel)
		.Gap(Single)
		.Add(Small, "Mana cost: ")
		.Num(Mana, ManaCost)
		.Gap(Single)
		.Add(Small, "Cooldown: ")
		.Num(Cooldown, CooldownCost)
		.Gap(Double)
		// Description.
		.Add(Default, "Emits ")
		.Num(Default, FMath::Min(NextLevel, MaxNumTargets))
		.Add(Default, " beams of lightning, connecting with the target, repeatedly causing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " lightning damage with a chance to stun.")
		.Get();
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

	LISTEN_TO_TARGET_DEATH(MouseHitActor, OnPrimaryTargetDied);
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

	TArray<AActor*> Result = UP16AbilitySystemLibrary::GetClosestTargets(NumTargets, MoveTemp(OverlappingActors), MouseHitLocation);
	for (AActor* Target : Result)
	{
		LISTEN_TO_TARGET_DEATH(Target, OnAdditionalTargetDied)
	}

	return Result;
}
