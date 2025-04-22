// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Niagara/P16DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/P16CombatInterface.h"

UP16DebuffNiagaraComponent::UP16DebuffNiagaraComponent()
{}

void UP16DebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP16DebuffNiagaraComponent::ListenToChanges(UAbilitySystemComponent* InAbilitySystem)
{
	InAbilitySystem->RegisterGameplayTagEvent(Tag).AddUObject(this, &ThisClass::OnDebuffTagChanged);
}

void UP16DebuffNiagaraComponent::OnDebuffTagChanged(FGameplayTag CallbackTag, const int32 NewCount)
{
	const TScriptInterface<IP16CombatInterface> CombatInterface = GetOwner();

	ToggleEffect(NewCount > 0
		&& CombatInterface
		&& !IP16CombatInterface::Execute_GetIsDead(GetOwner()));
}
