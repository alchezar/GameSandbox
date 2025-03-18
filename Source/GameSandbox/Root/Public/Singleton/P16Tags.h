// Copyright © 2025, Ivan Kinder

#pragma once

#include "GameplayTagContainer.h"
#include "P16Tags.generated.h"

/*
///
/// @struct  FP16GameplayTags
/// @brief   Represents all the gameplay tags used by the Project 16.
/// @details The place where all the gameplay tags of the Project 16 are stored.
///
USTRUCT(BlueprintType)
struct FP16GameplayTags
{
	GENERATED_BODY()

	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Vigor;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;

	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Physical;

	FGameplayTag Damage;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	FGameplayTag Effect_HitReact;
	FGameplayTag Event_Montage_Fireball;
	FGameplayTag Input_MouseButtonLeft;
	FGameplayTag Input_MouseButtonRight;
	FGameplayTag Input_Number1;
	FGameplayTag Input_Number2;
	FGameplayTag Input_Number3;
	FGameplayTag Input_Number4;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
};
*/

USTRUCT(BlueprintType)
struct FP16TagAbility
{
	GENERATED_BODY()

	FGameplayTag AttackTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttributePrimary
{
	GENERATED_BODY()

	FGameplayTag IntelligenceTag;
	FGameplayTag ResilienceTag;
	FGameplayTag StrengthTag;
	FGameplayTag VigorTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttributeSecondary
{
	GENERATED_BODY()

	FGameplayTag ArmorTag;
	FGameplayTag ArmorPenetrationTag;
	FGameplayTag BlockChanceTag;
	FGameplayTag CriticalHitChanceTag;
	FGameplayTag CriticalHitDamageTag;
	FGameplayTag CriticalHitResistanceTag;
	FGameplayTag HealthRegenerationTag;
	FGameplayTag ManaRegenerationTag;
	FGameplayTag MaxHealthTag;
	FGameplayTag MaxManaTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttributeVital
{
	GENERATED_BODY()

	FGameplayTag HealthTag;
	FGameplayTag ManaTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttributeResistance
{
	GENERATED_BODY()

	FGameplayTag ArcaneTag;
	FGameplayTag FireTag;
	FGameplayTag LightningTag;
	FGameplayTag PhysicalTag;

};

USTRUCT(BlueprintType)
struct FP16TagAttributeDamage
{
	GENERATED_BODY()

	FGameplayTag Tag;
	FGameplayTag ArcaneTag;
	FGameplayTag FireTag;
	FGameplayTag LightningTag;
	FGameplayTag PhysicalTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttribute
{
	GENERATED_BODY()

	FP16TagAttributePrimary    Primary;
	FP16TagAttributeSecondary  Secondary;
	FP16TagAttributeVital      Vital;
	FP16TagAttributeResistance Resistance;
};

USTRUCT(BlueprintType)
struct FP16TagEffect
{
	GENERATED_BODY()

	FGameplayTag HitReactTag;
};

USTRUCT(BlueprintType)
struct FP16TagEventMontage
{
	GENERATED_BODY()

	FGameplayTag FireballTag;
};

USTRUCT(BlueprintType)
struct FP16TagEvent
{
	GENERATED_BODY()

	FP16TagEventMontage Montage;
};

USTRUCT(BlueprintType)
struct FP16TagInput
{
	GENERATED_BODY()

	FGameplayTag MouseButtonLeftTag;
	FGameplayTag MouseButtonRightTag;
	FGameplayTag Number1Tag;
	FGameplayTag Number2Tag;
	FGameplayTag Number3Tag;
	FGameplayTag Number4Tag;
};

USTRUCT(BlueprintType)
struct FP16TagMaps
{
	GENERATED_BODY()

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
};

USTRUCT(BlueprintType)
struct FP16Tags
{
	GENERATED_BODY()

	FP16TagAbility         Ability;
	FP16TagAttribute       Attribute;
	FP16TagAttributeDamage Damage;
	FP16TagEffect          Effect;
	FP16TagEvent           Event;
	FP16TagInput           Input;

	FP16TagMaps Maps;
};
