// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P17AbilitySystemComponent.h"

#include "Project17.h"
#include "AbilitySystem/Abilities/P17HeroGameplayAbility.h"

UP17AbilitySystemComponent::UP17AbilitySystemComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UP17AbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	WARN_RETURN_IF(!InInputTag.IsValid(),)

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))

		// Activate the ability (previously added).
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UP17AbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{}

void UP17AbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FP17HeroAbilitySet>& InWeaponAbilities, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles, const int32 InLevel)
{
	RETURN_IF(InWeaponAbilities.IsEmpty(),)

	for (const FP17HeroAbilitySet& AbilitySet : InWeaponAbilities)
	{
		CONTINUE_IF(!AbilitySet.IsValid())

		FGameplayAbilitySpec AbilitySpec {AbilitySet.Ability, InLevel};
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.Add(GiveAbility(AbilitySpec));
	}
}

void UP17AbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	RETURN_IF(InGrantedAbilitySpecHandles.IsEmpty(),)

	for (const FGameplayAbilitySpecHandle& AbilitySpec : InGrantedAbilitySpecHandles)
	{
		CONTINUE_IF(!AbilitySpec.IsValid())
		ClearAbility(AbilitySpec);
	}

	InGrantedAbilitySpecHandles.Empty();
}

bool UP17AbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag AbilityTag)
{
	WARN_RETURN_IF(!AbilityTag.IsValid(), false)

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTag.GetSingleTagContainer(), FoundAbilitySpecs);
	RETURN_IF(FoundAbilitySpecs.IsEmpty(), false)

	const FGameplayAbilitySpec* AbilitySpec = FoundAbilitySpecs[0];
	RETURN_IF(!AbilitySpec || AbilitySpec->IsActive(), false)

	return TryActivateAbility(AbilitySpec->Handle);
}
