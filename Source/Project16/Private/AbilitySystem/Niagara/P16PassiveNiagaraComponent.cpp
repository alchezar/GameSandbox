// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Niagara/P16PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Character/P16CharacterBase.h"

UP16PassiveNiagaraComponent::UP16PassiveNiagaraComponent()
{}

void UP16PassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
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
