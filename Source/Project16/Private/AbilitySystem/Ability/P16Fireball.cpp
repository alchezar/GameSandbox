// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16Fireball.h"

#include "Project16.h"

FString UP16Fireball::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 DamageValue  = DamageInfo.Damage.GetValueAtLevel(CurrentLevel);
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
			.Add(Default, "a fireball, exploding on impact and dealing: ")
			.Get();
	}
	else
	{
		Description += FRichString {}
			.Num(Default, FMath::Min(CurrentLevel, NumProjectiles))
			.Add(Default, " balls of fire, exploding on impact and dealing: ")
			.Get();
	}

	Description += FRichString {}
		.Num(Damage, DamageValue)
		.Add(Default, " fire damage with a chance to burn.")
		.Get();

	return Description;
}

FString UP16Fireball::GetDescriptionNextLevel(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	const int32 NextLevel    = CurrentLevel + 1;
	const int32 DamageValue  = DamageInfo.Damage.GetValueAtLevel(NextLevel);
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
		.Num(Default, FMath::Min(NextLevel, NumProjectiles))
		.Add(Default, " balls of fire, exploding on impact and dealing: ")
		.Num(Damage, DamageValue)
		.Add(Default, " fire damage with a chance to burn.")
		.Get();
}
