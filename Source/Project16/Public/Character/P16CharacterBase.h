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
	virtual FVector       GetCombatSocketLocation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void          Die() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	virtual void InitAbilityActorInfo();
	virtual void InitDefaultAttributes() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_Die();

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InGameplayEffect, const float InLevel = 1.f) const;
	void AddCharacterAbilities() const;
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& InDynamicMaterials);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USkeletalMeshComponent> Weapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName HandSocketName = "P16_Hand";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName CombatSocketName = "TipSocket";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	TObjectPtr<UAnimMontage> HitReactMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Abilities")
	TArray<TSubclassOf<UP16GameplayAbility>> StartupAbilities = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Dissolve")
	TObjectPtr<UMaterialInterface> BodyDissolveMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Dissolve")
	TObjectPtr<UMaterialInterface> WeaponDissolveMaterial = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
