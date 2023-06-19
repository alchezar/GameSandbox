// Copyright (C) 2023, IKinder

#include "AREffectSlot.h"

#include "Components/Image.h"
#include "P6/Ability/ARAbilityEffect.h"
#include "P6/Component/ARAbilityComponent.h"

void UAREffectSlot::NativeConstruct()
{
	Super::NativeConstruct();
	ChangeEffectIcon();	
}

void UAREffectSlot::SetAbilityEffect(UARAbilityEffect* Effect)
{
	AbilityEffect = Effect;
}

void UAREffectSlot::ChangeEffectIcon()
{
	if (!AbilityEffect) return;
	
	UTexture2D* Icon = AbilityEffect->GetIcon();
	if (!Icon) return;
	
	EffectImage->SetBrushFromTexture(Icon);
}

void UAREffectSlot::BindDelegate()
{
	if (!AbilityEffect) return;

	UARAbilityComponent* AbilityComponent = AbilityEffect->GetAbilityComponent();
	if (!AbilityComponent) return;

	AbilityComponent->OnAbilityStopped.AddDynamic(this, &ThisClass::OnAbilityStoppedHandle);
}

void UAREffectSlot::OnAbilityStoppedHandle(UARAbilityComponent* AbilityComp, UARAbility* Ability)
{
	if (AbilityEffect != Ability) return;	
	RemoveFromParent();
}
