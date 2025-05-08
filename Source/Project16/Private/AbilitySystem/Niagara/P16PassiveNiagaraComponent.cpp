// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Niagara/P16PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Character/P16CharacterBase.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

UP16PassiveNiagaraComponent::UP16PassiveNiagaraComponent()
{}

void UP16PassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	// On load game, abilities are given before listening to changes, so we need to toggle effect manually.
	UP16AbilitySystemComponent* AbilitySystemComponent = Cast<UP16AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
	if (AbilitySystemComponent
		&& AbilitySystemComponent->GetIsStartupAbilitiesGiven()
		&& AbilitySystemComponent->GetStatusFromAbilityTag(Tag) == FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.EquippedTag)
	{
		ToggleEffect(true);
	}
}

void UP16PassiveNiagaraComponent::ListenToChanges(UAbilitySystemComponent* InAbilitySystem)
{
	UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(InAbilitySystem);
	EARLY_RETURN_IF(!AbilitySystem)
	AbilitySystem->OnActivatePassiveEffect.AddDynamic(this, &ThisClass::OnPassiveActivate);
}

void UP16PassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate)
{
	EARLY_RETURN_IF(!Tag.MatchesTagExact(AbilityTag))
	ToggleEffect(bActivate);
}
