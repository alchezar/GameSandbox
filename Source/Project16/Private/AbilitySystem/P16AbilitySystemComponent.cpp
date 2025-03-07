// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

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
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnEffectAppliedCallback);
}

void UP16AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UP16GameplayAbility>& AbilityClass : StartupAbilities)
	{
		// // One way to give and activate an ability.
		// const FGameplayAbilitySpecHandle AbilitySpecHandle = GiveAbility(FGameplayAbilitySpec{AbilityClass});
		// TryActivateAbility(AbilitySpecHandle);

		// Another way to give and activate an ability.
		FGameplayAbilitySpec AbilitySpec = {AbilityClass};
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UP16AbilitySystemComponent::OnEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	OnEffectApplied.Broadcast(TagContainer);
}
