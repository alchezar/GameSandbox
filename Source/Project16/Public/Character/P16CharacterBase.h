// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"
#include "P16CharacterBase.generated.h"

class UP16GameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class PROJECT16_API AP16CharacterBase : public ACharacter, public IP16CombatInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16CharacterBase();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual FVector GetCombatSocketLocation() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	virtual void InitAbilityActorInfo();
	virtual void InitDefaultAttributes() const;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InGameplayEffect, const float InLevel = 1.f) const;
	void AddCharacterAbilities() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USkeletalMeshComponent> Weapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName HandSocketName = "P16_Hand";
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName CombatSocketName = "TipSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Abilities")
	TArray<TSubclassOf<UP16GameplayAbility>> StartupAbilities = {};

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
