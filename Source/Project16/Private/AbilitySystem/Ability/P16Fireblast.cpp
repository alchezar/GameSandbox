// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16Fireblast.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "Actor/P16BlastBall.h"

FString UP16Fireblast::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = GetDamageAtLevel(CurrentLevel);
	const float ManaCost     = GetManaCost(CurrentLevel);
	const float CooldownTime = GetCooldownTime(CurrentLevel);

	FString Description = FRichString {}
		// Title.
		.Add(Title, "FIRE BLAST")
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
		.Num(Default, NumBalls)
		.Add(Default, " balls of fire in all directions, each coming back and exploding upon return, causing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " radial fire damage with a chance to burn.")
		.Get();

	return Description;
}

FString UP16Fireblast::GetDescriptionNextLevel(const int32 CurrentLevel)
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
		.Num(Default, NumBalls)
		.Add(Default, " balls of fire in all directions, each coming back and exploding upon return, causing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " radial fire damage with a chance to burn.")
		.Get();
}

TArray<AP16BlastBall*> UP16Fireblast::SpawnBlastBalls(const FGameplayTag& SocketTag) const
{
	TArray<AP16BlastBall*> Result = {};
	Result.Reserve(NumBalls);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AActor* Owner  = GetOwningActorFromActorInfo();

	TArray<FRotator> Rotations = UP16AbilitySystemLibrary::EvenlySpacedRotators(Avatar->GetActorForwardVector(), FVector::UpVector, 360.f, NumBalls);

	for (int32 Index = 0; Index < NumBalls; ++Index)
	{
		const FTransform Transform = {Rotations[Index], Avatar->GetActorLocation()};
		AP16BlastBall*   BlastBall = GetWorld()->SpawnActorDeferred<AP16BlastBall>(
			ProjectileClass,
			Transform,
			Owner,
			Cast<APawn>(Avatar),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		BlastBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		BlastBall->FinishSpawning(Transform, true);
		Result.Add(BlastBall);
	}

	return Result;
}
