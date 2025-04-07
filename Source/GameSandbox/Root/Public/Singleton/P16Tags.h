// Copyright © 2025, Ivan Kinder

#pragma once

#include "GameplayTagContainer.h"
#include "P16Tags.generated.h"

USTRUCT(BlueprintType)
struct FP16TagAbilityFire
{
	GENERATED_BODY()

	FGameplayTag FireballTag;
};

USTRUCT()
struct FP16TagAbilityLightning
{
	GENERATED_BODY()

	FGameplayTag ElectrocuteTag;
};

USTRUCT(BlueprintType)
struct FP16TagAbilityStatus
{
	GENERATED_BODY()

	FGameplayTag Tag;
	FGameplayTag LockedTag;
	FGameplayTag EligibleTag;
	FGameplayTag UnlockedTag;
	FGameplayTag EquippedTag;
};

USTRUCT(BlueprintType)
struct FP16TagAbilityType
{
	GENERATED_BODY()

	FGameplayTag OffensiveTag;
	FGameplayTag PassiveTag;
	FGameplayTag NoneTag;
};

USTRUCT(BlueprintType)
struct FP16TagAbility
{
	GENERATED_BODY()

	FGameplayTag Tag;
	FGameplayTag NoneTag;
	FGameplayTag AttackTag;
	FGameplayTag SummonTag;
	FGameplayTag HitReactTag;

	FP16TagAbilityFire      Fire;
	FP16TagAbilityLightning Lightning;
	FP16TagAbilityStatus    Status;
	FP16TagAbilityType      Type;
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
struct FP16TagAttributeMeta
{
	GENERATED_BODY()

	FGameplayTag IncomingXPTag;
};

USTRUCT(BlueprintType)
struct FP16TagAttribute
{
	GENERATED_BODY()

	FP16TagAttributePrimary    Primary;
	FP16TagAttributeSecondary  Secondary;
	FP16TagAttributeVital      Vital;
	FP16TagAttributeResistance Resistance;
	FP16TagAttributeMeta       Meta;
};

USTRUCT(BlueprintType)
struct FP16TagCooldownFire
{
	GENERATED_BODY()

	FGameplayTag FireballTag;
};

USTRUCT(BlueprintType)
struct FP16TagCooldownLightning
{
	GENERATED_BODY()

	FGameplayTag ElectrocuteTag;
};

USTRUCT(BlueprintType)
struct FP16TagCooldown
{
	GENERATED_BODY()

	FP16TagCooldownFire      Fire;
	FP16TagCooldownLightning Lightning;
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
struct FP16TagCombatSocket
{
	GENERATED_BODY()

	FGameplayTag WeaponTag;
	FGameplayTag LeftHandTag;
	FGameplayTag RightHandTag;
	FGameplayTag TailTag;
};

USTRUCT(BlueprintType)
struct FP16TagMontageAttack
{
	GENERATED_BODY()

	FGameplayTag FirstTag;
	FGameplayTag SecondTag;
	FGameplayTag ThirdTag;
	FGameplayTag FourthTag;
};

USTRUCT(BlueprintType)
struct FP16TagMontage
{
	GENERATED_BODY()

	FP16TagMontageAttack Attack;
};

USTRUCT(BlueprintType)
struct FP16TagInput
{
	GENERATED_BODY()

	FGameplayTag Tag;
	FGameplayTag MouseButtonLeftTag;
	FGameplayTag MouseButtonRightTag;
	FGameplayTag Number1Tag;
	FGameplayTag Number2Tag;
	FGameplayTag Number3Tag;
	FGameplayTag Number4Tag;
	FGameplayTag Passive1Tag;
	FGameplayTag Passive2Tag;
};

USTRUCT(BlueprintType)
struct FP16TagMaps
{
	GENERATED_BODY()

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
};

///
/// @struct  FP16Tags
/// @brief   Represents all the gameplay tags used by the Project 16.
/// @details The place where all the gameplay tags of the Project 16 are stored.
///
USTRUCT(BlueprintType)
struct FP16Tags
{
	GENERATED_BODY()

	FP16TagAbility         Ability;
	FP16TagAttribute       Attribute;
	FP16TagCooldown        Cooldown;
	FP16TagAttributeDamage Damage;
	FP16TagEffect          Effect;
	FP16TagEvent           Event;
	FP16TagCombatSocket    CombatSocket;
	FP16TagMontage         Montage;
	FP16TagInput           Input;

	FP16TagMaps Maps;
};
