// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16Fireball.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "Actor/P16Projectile.h"
#include "Interface/P16CombatInterface.h"

FString UP16Fireball::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = GetDamageAtLevel(CurrentLevel);
	const float ManaCost     = GetManaCost(CurrentLevel);
	const float CooldownTime = GetCooldownTime(CurrentLevel);

	FString Description = FRichString {}
		// Title.
		.Add(Title, "FIREBALL")
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
		.Add(Default, "Launches ")
		.Get();

	if (CurrentLevel == 1)
	{
		Description += FRichString {}
			.Add(Default, "a fireball")
			.Get();
	}
	else
	{
		Description += FRichString {}
			.Num(Default, FMath::Min(CurrentLevel, MaxNumProjectiles))
			.Add(Default, " balls of fire")
			.Get();
	}

	Description += FRichString {}
		.Add(Default, ", exploding on impact and dealing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " fire damage with a chance to burn.")
		.Get();

	return Description;
}

FString UP16Fireball::GetDescriptionNextLevel(const int32 CurrentLevel)
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
		.Add(Default, "Launches ")
		.Num(Default, FMath::Min(NextLevel, MaxNumProjectiles))
		.Add(Default, " balls of fire, exploding on impact and dealing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " fire damage with a chance to burn.")
		.Get();
}

void UP16Fireball::SpawnProjectiles(const FVector& InTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, const float AdditionalPitch)
{
	if (!ProjectileClass)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

	AActor* Avatar = GetAvatarActorFromActorInfo();
	EARLY_RETURN_IF(!Avatar)
	const bool bServer = Avatar->HasAuthority();
	EARLY_RETURN_IF(!bServer)

	FTransform SpawnTransform = FTransform::Identity;
	EARLY_RETURN_IF(!Avatar->Implements<UP16CombatInterface>())

	const FVector SocketLocation  = IP16CombatInterface::Execute_GetCombatSocketLocation(Avatar, SocketTag);
	FRotator      ForwardRotation = (InTargetLocation - SocketLocation).Rotation();
	ForwardRotation.Pitch += AdditionalPitch;

	const FVector          Forward        = ForwardRotation.Vector();
	const int32            NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	const TArray<FRotator> Rotations      = UP16AbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpreadAngle, NumProjectiles);

	for (const FRotator& Rotation : Rotations)
	{
		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);

		AP16Projectile* Projectile = GetWorld()->SpawnActorDeferred<AP16Projectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		CONTINUE_IF(!Projectile)

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->UpdateHomingTarget(HomingTarget, InTargetLocation);
		Projectile->FinishSpawning(SpawnTransform);
	}
}
