// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Niagara/P16NiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/P16CombatInterface.h"

UP16NiagaraComponent::UP16NiagaraComponent()
{
	bAutoActivate                     = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UP16NiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	const TScriptInterface<IP16CombatInterface> CombatInterface        = GetOwner();
	UAbilitySystemComponent*                    AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	// Listen to changes if the ability system component is already valid.
	if (AbilitySystemComponent)
	{
		ListenToChanges(AbilitySystemComponent);
	}
	// Or start listening to changes after the ability system component is registered.
	else if (CombatInterface)
	{
		CombatInterface->GetOnAbilitySystemRegisteredDelegate().AddUObject(this, &ThisClass::ListenToChanges);
	}

	// Make sure we listen to owner's death delegate.
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnDeathCallback);
	}
}

void UP16NiagaraComponent::Init(const FGameplayTag& InTag)
{
	Tag = InTag;
}

void UP16NiagaraComponent::ToggleEffect(const bool bActivate)
{
	if (bActivate)
	{
		Activate();
		return;
	}

	Deactivate();
}

void UP16NiagaraComponent::ListenToChanges(UAbilitySystemComponent* InAbilitySystem)
{
	//
}

void UP16NiagaraComponent::OnDeathCallback(AActor* DeadActor)
{
	ToggleEffect(false);
}
