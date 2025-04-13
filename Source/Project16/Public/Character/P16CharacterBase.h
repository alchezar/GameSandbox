// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"
#include "Util/P16Type.h"
#include "P16CharacterBase.generated.h"

class UP16DebuffNiagaraComponent;
class UNiagaraSystem;
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
	virtual UAnimMontage*                           GetHitReactMontage_Implementation() override;
	virtual void                                    Die() override;
	virtual FVector                                 GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual bool                                    GetIsDead_Implementation() const override;
	virtual AActor*                                 GetAvatar_Implementation() override;
	virtual TArray<FP16TaggedMontage>               GetAttackMontages_Implementation() override;
	virtual FP16TaggedMontage                       GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual UNiagaraSystem*                         GetBloodEffect_Implementation() override;
	virtual int32                                   GetMinionCount_Implementation() override;
	virtual void                                    IncrementMinionCount_Implementation(const int32 Amount = 1) override;
	virtual EP16CharacterClass                      GetCharacterClass_Implementation() override;
	virtual FP16OnAbilitySystemRegisteredSignature& GetOnAbilitySystemRegisteredDelegate() override;
	virtual FP16OnDeathSignature&                   GetOnDeathDelegate() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	/// @brief Initialize ability actor info.
	/// @attention Super method should be called at the end of the overriden method.
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
public:
	FP16OnAbilitySystemRegisteredSignature OnAbilitySystemRegistered;
	FP16OnDeathSignature                   OnDeath;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USkeletalMeshComponent> Weapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16DebuffNiagaraComponent> BurnDebuff = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	EP16CharacterClass CharacterClass = EP16CharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	FName HandSocketName = "P16_Hand";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	TMap<FGameplayTag, FName> CombatSocketNameMap = {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	TObjectPtr<UAnimMontage> HitReactMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Combat")
	TArray<FP16TaggedMontage> AttackMontages = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Abilities")
	TArray<TSubclassOf<UP16GameplayAbility>> StartupAbilities = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Dissolve")
	TObjectPtr<UMaterialInterface> BodyDissolveMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Dissolve")
	TObjectPtr<UMaterialInterface> WeaponDissolveMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Effect")
	TObjectPtr<UNiagaraSystem> BloodEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Effect")
	TObjectPtr<USoundBase> DeathSound = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	int32 MinionCount = 0;

private:
	bool bDead = false;
};
