// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Debuff/P16DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/P16CombatInterface.h"

UP16DebuffNiagaraComponent::UP16DebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UP16DebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	const TScriptInterface<IP16CombatInterface> CombatInterface        = GetOwner();
	UAbilitySystemComponent*                    AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	// Make sure we listen to debuff tag changes, regardless of whether the ability system is currently registered at the moment or not yet.
	if (AbilitySystemComponent)
	{
		ListenToDebuffTagChanges(AbilitySystemComponent);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnAbilitySystemRegisteredDelegate().AddUObject(this, &ThisClass::ListenToDebuffTagChanges);
	}

	// Make sure we listen to owner's death delegate.
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnDeathCallback);
	}
}

void UP16DebuffNiagaraComponent::OnDebuffTagChanged(FGameplayTag CallbackTag, const int32 NewCount)
{
	const TScriptInterface<IP16CombatInterface> CombatInterface = GetOwner();

	if (NewCount > 0 && CombatInterface && !IP16CombatInterface::Execute_GetIsDead(GetOwner()))
	{
		Activate();
		return;
	}

	Deactivate();
}

void UP16DebuffNiagaraComponent::ListenToDebuffTagChanges(UAbilitySystemComponent* AbilitySystemComponent)
{
	AbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag).AddUObject(this, &ThisClass::OnDebuffTagChanged);
}

void UP16DebuffNiagaraComponent::OnDeathCallback(AActor* DeadActor)
{
	Deactivate();
}
