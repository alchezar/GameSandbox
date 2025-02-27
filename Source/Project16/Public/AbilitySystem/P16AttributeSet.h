// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
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

UCLASS()
class PROJECT16_API UP16AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP16AttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	/* ------------------------------ Fields ------------------------------- */
public:
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadWrite, Category = "C++ | Vital")
	FGameplayAttributeData Health = {75.f};
	ATTRIBUTE_ACCESSORS(Health)

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadWrite, Category = "C++ | Vital")
	FGameplayAttributeData MaxHealth = {100.f};
	ATTRIBUTE_ACCESSORS(MaxHealth)

	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Mana = {25.f};
	ATTRIBUTE_ACCESSORS(Mana)

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData MaxMana = {50.f};
	ATTRIBUTE_ACCESSORS(MaxMana)
};
