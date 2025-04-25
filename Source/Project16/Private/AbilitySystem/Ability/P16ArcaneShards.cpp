// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16ArcaneShards.h"
#include "Project16.h"

FString UP16ArcaneShards::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = GetDamageAtLevel(CurrentLevel);
	const float ManaCost     = GetManaCost(CurrentLevel);
	const float CooldownTime = GetCooldownTime(CurrentLevel);

	FString Description = FRichString {}
		// Title.
		.Add(Title, "ARCANE SHARDS")
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
		.Add(Default, "Summon ")
		.Get();

	if (CurrentLevel == 1)
	{
		Description += FRichString {}
			.Add(Default, "a shard ")
			.Get();
	}
	else
	{
		Description += FRichString {}
			.Num(Default, FMath::Min(CurrentLevel, MaxPointsNum))
			.Add(Default, " shards ")
			.Get();
	}

	Description += FRichString {}
		.Add(Default, "of arcane energy, causing radial arcane damage of ")
		.Num(Damage, DamageValue)
		.Add(Default, " at the shard origin.")
		.Get();

	return Description;
}

FString UP16ArcaneShards::GetDescriptionNextLevel(const int32 CurrentLevel)
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
		.Add(Default, "Summon ")
		.Num(Default, FMath::Min(NextLevel, MaxPointsNum))
		.Add(Default, " shards of arcane energy, causing radial arcane damage of ")
		.Num(Damage, DamageValue)
		.Add(Default, " at the shard origin.")
		.Get();
}
