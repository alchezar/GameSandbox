// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P17AbilitySystemComponent.h"

#include "Project17.h"

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
