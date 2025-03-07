// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#define DEFINE_ONREP_GAMEPLAYATTRIBUTE(ClassName, PropertyName)                             \
void ClassName::OnRep_##PropertyName(const FGameplayAttributeData& Old##PropertyName) const \
{                                                                                           \
	GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, Old##PropertyName)                 \
}

UP16AttributeSet::UP16AttributeSet()
{}

void UP16AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// clang-format off
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience,   COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor,        COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor,                 COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration,      COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance,           COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration,    COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration,      COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth,             COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana,               COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana,   COND_None, REPNOTIFY_Always)
	// clang-format on
}

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Strength)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Intelligence)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Resilience)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Vigor)

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Armor)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, ArmorPenetration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, BlockChance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitChance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitDamage)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitResistance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, HealthRegeneration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, ManaRegeneration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, MaxHealth)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, MaxMana)

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Health)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Mana)

void UP16AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp values before the changes are actually happens is a bad idea.
	// if (Attribute == GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	// }
	// else if (Attribute == GetManaAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	// }
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
