// Copyright (C) 2023, IKinder

#include "ARAbilityEffectThorn.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Util/ARFuncLibrary.h"

UARAbilityEffectThorn::UARAbilityEffectThorn()
{
	bAutostart = true;
}

void UARAbilityEffectThorn::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	UARAttributesComponent* AttributeComp = UARAttributesComponent::GetAttributes(Instigator);
	if (!AttributeComp) return;

	AttributeComp->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void UARAbilityEffectThorn::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);

	UARAttributesComponent* AttributeComp = UARAttributesComponent::GetAttributes(Instigator);
	if (!AttributeComp) return;

	AttributeComp->AROnHealthChanged.RemoveDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void UARAbilityEffectThorn::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = OwningComp->GetOwner();
	if (!OwningActor) return;

	if (Delta < 0.f && OwningActor != InstigatorActor)
	{
		const int32 ReflectAmount = FMath::Abs(FMath::RoundToInt(Delta * ReflectFraction));
		if (!ReflectAmount) return;

		UARFuncLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectAmount);
	}
}
