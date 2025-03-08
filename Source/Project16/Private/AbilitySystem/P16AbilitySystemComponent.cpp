// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

#include "Project16.h"
#include "AbilitySystem/Ability/P16GameplayAbility.h"

UP16AbilitySystemComponent::UP16AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP16AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP16AbilitySystemComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP16AbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::Client_OnEffectAppliedCallback);
}

void UP16AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UP16GameplayAbility>& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec       AbilitySpec = {AbilityClass};
		const UP16GameplayAbility* Ability     = Cast<UP16GameplayAbility>(AbilitySpec.Ability);
		CONTINUE_IF(!Ability)

		AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->StartupInputTag);
		GiveAbility(AbilitySpec);
	}
}

void UP16AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))

		AbilitySpecInputPressed(AbilitySpec);
		CONTINUE_IF(AbilitySpec.IsActive())

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UP16AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))

		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UP16AbilitySystemComponent::Client_OnEffectAppliedCallback_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	OnEffectApplied.Broadcast(TagContainer);
}
