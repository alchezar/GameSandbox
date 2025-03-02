// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UP16AttributeSet::UP16AttributeSet()
{}

void UP16AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always)
}

void UP16AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp values before the changes are actually happens is a bad idea.
}

void UP16AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FP16EffectProperties Properties = GetEffectProperties(Data);

	// Correct place to clamp attribute values.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UP16AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth)
}

void UP16AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth)
}

void UP16AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UP16AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}

FP16EffectProperties UP16AttributeSet::GetEffectProperties(const FGameplayEffectModCallbackData& InData) const
{
	FP16EffectProperties OutProperties;
	OutProperties.EffectContext = InData.EffectSpec.GetContext();

	// Source fields.
	OutProperties.SourceAbilitySystem = OutProperties.EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	if (!OutProperties.SourceAbilitySystem || !OutProperties.SourceAbilitySystem->AbilityActorInfo)
	{
		return OutProperties;
	}

	OutProperties.SourceAvatarActor = OutProperties.SourceAbilitySystem->AbilityActorInfo->AvatarActor.Get();
	if (!OutProperties.SourceAvatarActor)
	{
		return OutProperties;
	}

	OutProperties.SourceController = OutProperties.SourceAbilitySystem->AbilityActorInfo->PlayerController.Get();

	if (!OutProperties.SourceController && OutProperties.SourceAvatarActor)
	{
		if (const APawn* SourcePawn = Cast<APawn>(OutProperties.SourceAvatarActor))
		{
			OutProperties.SourceController = SourcePawn->GetController();
		}
	}

	if (!OutProperties.SourceController)
	{
		return OutProperties;
	}

	OutProperties.SourceCharacter = Cast<ACharacter>(OutProperties.SourceController->GetPawn());

	// Target fields.
	if (!InData.Target.AbilityActorInfo || !InData.Target.AbilityActorInfo->AvatarActor.Get())
	{
		return OutProperties;
	}

	OutProperties.TargetAvatarActor   = InData.Target.GetAvatarActor();
	OutProperties.TargetController    = InData.Target.AbilityActorInfo->PlayerController.Get();
	OutProperties.TargetCharacter     = Cast<ACharacter>(OutProperties.TargetAvatarActor);
	OutProperties.TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProperties.TargetAvatarActor.Get());

	return OutProperties;
}
