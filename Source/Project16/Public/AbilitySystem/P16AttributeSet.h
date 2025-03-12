// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Util/P16Type.h"
#include "P16AttributeSet.generated.h"

///
/// This defines a set of helper functions for accessing and initializing attributes,
/// to avoid having to manually write these functions.
/// It would create the following functions, for attribute Health
///
/// static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
/// FORCEINLINE float UMyHealthSet::GetHealth() const;
/// FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
/// FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
///
#define ATTRIBUTE_ACCESSORS(PropertyName)                      \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ThisClass, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/// ----------------------------------------------------------------------------
/// @class   UP16AttributeSet
/// @brief   Represents an attribute set for a character.
/// @details Extends the base class with custom attributes, property accessors
///          and replication logic.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16AttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	FP16EffectProperties GetEffectProperties(const FGameplayEffectModCallbackData& InData) const;
	void                 HandleIncomingDamage(const FP16EffectProperties& Properties);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	/// ------------------------------------------------------------------------
	/// @section Primary attributes.
	/// ------------------------------------------------------------------------

	/// @brief Increase physical damage.
	UPROPERTY(ReplicatedUsing = OnRep_Strength, BlueprintReadWrite, Category = "C++ | Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(Strength)

	/// @brief Increase magical damage.
	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, BlueprintReadWrite, Category = "C++ | Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(Intelligence)

	/// @brief Increase armor and armor penetration.
	UPROPERTY(ReplicatedUsing = OnRep_Resilience, BlueprintReadWrite, Category = "C++ | Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(Resilience)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_Vigor, BlueprintReadWrite, Category = "C++ | Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(Vigor)

	/// ------------------------------------------------------------------------
	/// @section Secondary Attributes.
	/// ------------------------------------------------------------------------

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_Armor, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(Armor)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(ArmorPenetration)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_BlockChance, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(BlockChance)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(CriticalHitChance)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitDamage, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(CriticalHitDamage)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitResistance, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(CriticalHitResistance)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_HealthRegeneration, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(HealthRegeneration)

	/// @brief Increase health.
	UPROPERTY(ReplicatedUsing = OnRep_ManaRegeneration, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(ManaRegeneration)

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(MaxHealth)

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadWrite, Category = "C++ | Secondary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(MaxMana)

	/// ------------------------------------------------------------------------
	/// @section Vital Attributes.
	/// ------------------------------------------------------------------------

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadWrite, Category = "C++ | Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(Health)

	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadWrite, Category = "C++ | Vital")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(Mana)

	/// ------------------------------------------------------------------------
	/// @section Meta Attributes.
	/// ------------------------------------------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(IncomingDamage)
};
