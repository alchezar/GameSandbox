// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"
#include "Util/P16Type.h"
#include "P16CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UNiagaraSystem;
class UP16DebuffNiagaraComponent;
class UP16GameplayAbility;
class UP16PassiveNiagaraComponent;

UCLASS(Abstract)
class PROJECT16_API AP16CharacterBase : public ACharacter, public IP16CombatInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16CharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaSeconds) override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual UAnimMontage*                           GetHitReactMontage_Implementation() override;
	virtual void                                    Die(const FVector& DeathImpulse) override;
	virtual FVector                                 GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag) override;
	virtual bool                                    GetIsDead_Implementation() const override;
	virtual bool                                    GetBeingShocked_Implementation() const override;
	virtual AActor*                                 GetAvatar_Implementation() override;
	virtual TArray<FP16TaggedMontage>               GetAttackMontages_Implementation() override;
	virtual FP16TaggedMontage                       GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual UNiagaraSystem*                         GetBloodEffect_Implementation() override;
	virtual int32                                   GetMinionCount_Implementation() override;
	virtual void                                    IncrementMinionCount_Implementation(const int32 Amount = 1) override;
	virtual EP16CharacterClass                      GetCharacterClass_Implementation() override;
	virtual FP16OnAbilitySystemRegisteredSignature& GetOnAbilitySystemRegisteredDelegate() override;
	virtual FP16OnDeathSignature&                   GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent*                 GetWeapon_Implementation() override;
	virtual void                                    SetBeingShocked_Implementation(const bool bIsShocked) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	/// @brief Initialize ability actor info.
	/// @attention Super method should be called at the end of the overriden method.
	virtual void InitAbilityActorInfo();
	virtual void InitDefaultAttributes() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_Die(const FVector& DeathImpulse);

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InGameplayEffect, const float InLevel = 1.f) const;
	void AddCharacterAbilities() const;
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& InDynamicMaterials);

	virtual void OnBurnTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);
	virtual void OnStunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);

private:
	UFUNCTION()
	void OnRep_Burned(const bool bOldBurned = false) const;
	UFUNCTION()
	void OnRep_Stunned(const bool bOldStunned = false) const;
	void OrientPassiveSpells() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	FP16OnAbilitySystemRegisteredSignature OnAbilitySystemRegistered;
	FP16OnDeathSignature                   OnDeath;

	UPROPERTY(ReplicatedUsing = "OnRep_Burned", BlueprintReadOnly, Category = "C++")
	bool bBurned = false;
	UPROPERTY(ReplicatedUsing = "OnRep_Stunned", BlueprintReadOnly, Category = "C++")
	bool bStunned = false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "C++")
	bool bBeingShocked = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USkeletalMeshComponent> Weapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16DebuffNiagaraComponent> BurnDebuff = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16DebuffNiagaraComponent> StunDebuff = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USceneComponent> EffectAttachComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16PassiveNiagaraComponent> HaloPassive = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16PassiveNiagaraComponent> LifePassive = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP16PassiveNiagaraComponent> ManaPassive = nullptr;

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

	int32 MinionCount   = 0;
	float BaseWalkSpeed = 0.f;

private:
	bool bDead = false;
};
