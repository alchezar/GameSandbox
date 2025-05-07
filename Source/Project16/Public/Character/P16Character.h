// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "P16CharacterBase.h"
#include "Interface/P16PlayerInterface.h"
#include "P16Character.generated.h"

class UP16SaveGame;
class AP16PlayerState;
class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT16_API AP16Character : public AP16CharacterBase, public IAbilitySystemInterface, public IP16PlayerInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16Character();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @name IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/// @name IP16CombatInterface
	virtual int32 GetPlayerLevel_Implementation() override;

	/// @name IP16PlayerInterface
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetLevelFor_Implementation(const int32 XP) const override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void  AddToXP_Implementation(const int32 XP) override;
	virtual void  AddToLevel_Implementation(const int32 DeltaLevel) override;
	virtual void  AddAttributePoints_Implementation(const int32 InAttributePoints) override;
	virtual void  AddSpellPoints_Implementation(const int32 InSpellPoints) override;
	virtual void  LevelUp_Implementation() override;
	virtual void  ToggleMagicCircle_Implementation(const bool bOn, UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void  SaveProgress_Implementation(const FName& CheckpointTag) override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UAttributeSet* GetAttributeSet() const;

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LevelUpParticles() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> Camera = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent = nullptr;

private:
	UPROPERTY()
	TObjectPtr<AP16PlayerState> OwnPlayerState = nullptr;
};
