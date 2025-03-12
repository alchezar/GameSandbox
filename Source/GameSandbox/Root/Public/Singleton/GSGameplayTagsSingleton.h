// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GSGameplayTagsSingleton.generated.h"

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
	FGameplayTag Damage;
	FGameplayTag Effect_HitReact;
	FGameplayTag Event_Montage_Fireball;
	FGameplayTag Input_MouseButtonLeft;
	FGameplayTag Input_MouseButtonRight;
	FGameplayTag Input_Number1;
	FGameplayTag Input_Number2;
	FGameplayTag Input_Number3;
	FGameplayTag Input_Number4;
};

///
/// @class   FGSGameplayTagsSingleton
/// @brief   Represents a singleton class for storing gameplay tags.
/// @details Instead of getting gameplay tags from FGameplayTag::RequestGameplayTag, we can get them from this class.
///
USTRUCT()
struct GAMESANDBOX_API FGSGameplayTagsSingleton
{
	GENERATED_BODY()

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD static const FGSGameplayTagsSingleton& Get() { return Singleton; }

	static void InitializeNativeGameplayTags();

private:
	static void InitializeProject16GameplayTags();

	/* ------------------------------ Fields ------------------------------- */
public:
	FP16GameplayTags P16Tags;

private:
	static FGSGameplayTagsSingleton Singleton;
};
