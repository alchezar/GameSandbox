// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16GameplayAbility.h"

#include "Project16.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "Interface/P16CombatInterface.h"
#include "Interface/P16InterfaceEnemy.h"

void UP16GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
AActor* UP16GameplayAbility::OrientToTarget()
{
	AActor* Target = IP16InterfaceEnemy::Execute_GetCombatTarget(GetAvatarActorFromActorInfo());
	EARLY_RETURN_VALUE_IF(!Target, nullptr)

	IP16CombatInterface::Execute_UpdateFacingTo(GetAvatarActorFromActorInfo(), Target->GetActorLocation());
	return Target;
}

FString UP16GameplayAbility::GetDescription(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	// return Default + "Current Level " + End + Level + FString::FormatAsNumber(CurrentLevel) + End + Default + " Causes some damage." + End;
	return FRichString {}
		.Add(Title, "SPELL")
		.Gap(Double)
		.Add(Small, "Current level ")
		.Num(Level, CurrentLevel)
		.Gap(Double)
		.Add(Default, "Causes some damage.")
		.Get();
}

FString UP16GameplayAbility::GetDescriptionNextLevel(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	// return Default + "Next Level " + End + Level + FString::FormatAsNumber(CurrentLevel + 1) + End + Default + " Causes much more damage." + End;
	return FRichString {}
		.Add(Title, "NEXT LEVEL")
		.Gap(Double)
		.Add(Small, "Next level ")
		.Num(Level, CurrentLevel + 1)
		.Gap(Double)
		.Add(Default, "Causes much more damage.")
		.Get();
}

FString UP16GameplayAbility::GetDescriptionLocked(const int32 CurrentLevel)
{
	using namespace P16::Rich;

	// FString Description = Title + "SPELL LOCKED" + End + TwoLines;
	// Description += Default + "Until level " + End + Level + FString::FormatAsNumber(CurrentLevel) + End;
	// return Default + "Spell locked until level: " + End + Level + FString::FormatAsNumber(CurrentLevel) + End;
	return FRichString {}
		.Add(Title, "SPELL LOCKED")
		.Gap(Double)
		.Add(Small, "Until level: ")
		.Num(Level, CurrentLevel)
		.Get();
}

float UP16GameplayAbility::GetManaCost(const float InLevel) const
{
	float ManaCost = 0.f;

	UGameplayEffect* Cost = GetCostGameplayEffect();
	EARLY_RETURN_VALUE_IF(!Cost, ManaCost)

	auto FindIf = [](const FGameplayModifierInfo& Modifier) -> bool
	{
		return Modifier.Attribute == UP16AttributeSet::GetManaAttribute();
	};
	const FGameplayModifierInfo* Modifier = Cost->Modifiers.FindByPredicate(FindIf);
	EARLY_RETURN_VALUE_IF(!Modifier, ManaCost)

	Modifier->ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, OUT ManaCost);
	return FMath::Abs(ManaCost);
}

float UP16GameplayAbility::GetCooldownTime(const float InLevel) const
{
	float CooldownTime = 0.f;

	const UGameplayEffect* Cooldown = GetCooldownGameplayEffect();
	EARLY_RETURN_VALUE_IF(!Cooldown, CooldownTime)

	Cooldown->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, OUT CooldownTime);
	return CooldownTime;
}
