// Copyright (C) 2023, IKinder

#include "AREffectContainer.h"
#include "Components/HorizontalBox.h"
#include "P6/Component/ARAbilityComponent.h"
#include "P6/UI/AREffectSlot.h"
#include "P6/Ability/ARAbilityEffect.h"

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