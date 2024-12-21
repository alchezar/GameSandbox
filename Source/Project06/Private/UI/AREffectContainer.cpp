// Copyright (C) 2023, IKinder

#include "UI/AREffectContainer.h"

#include "Ability/ARAbilityEffect.h"
#include "Component/ARAbilityComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/AREffectSlot.h"

bool UAREffectContainer::Initialize()
{
	return Super::Initialize();
}

void UAREffectContainer::NativeConstruct()
{
	Super::NativeConstruct();
	EffectBox->ClearChildren();
}

void UAREffectContainer::SetAbilityComponent(UARAbilityComponent* AbilityComp)
{
	AbilityComponent = AbilityComp;
	if (!AbilityComponent) return;

	AbilityComponent->OnAbilityStarted.AddDynamic(this, &ThisClass::OnAbilityStartedHandle);

	TArray<UARAbility*> Abilities = AbilityComponent->GetAbilities();
	if (Abilities.IsEmpty()) return;
	
	for (auto* Ability : Abilities)
	{
		OnAbilityStartedHandle(AbilityComponent, Ability);
	}
}

void UAREffectContainer::OnAbilityStartedHandle(UARAbilityComponent* AbilityComp, UARAbility* Ability)
{
	UARAbilityEffect* AbilityEffect = Cast<UARAbilityEffect>(Ability);
	if (!AbilityEffect) return;
	
	UAREffectSlot* EffectSlotInstance = CreateWidget<UAREffectSlot>(GetOwningPlayer(), EffectSlotClass);
	if (!EffectSlotInstance) return;

	EffectSlotInstance->SetAbilityEffect(AbilityEffect);
	EffectBox->AddChild(EffectSlotInstance);
}