// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Util/P16Util.h"
#include "P16PlayerState.generated.h"

class UP16LevelUpInfoDataAsset;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API AP16PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	UAttributeSet* GetAttributeSet() const;
	_NODISCARD
	int32 GetXP() const { return XP; };
	_NODISCARD
	int32 GetPlayerLevel() const { return Level; };
	_NODISCARD
	int32 GetAttributePoints() const { return AttributePoints; };
	_NODISCARD
	int32 GetSpellPoints() const { return SpellPoints; };

	void SetXP(const int32 NewXP);
	void SetLevel(const int32 NewLevel);

	void AddXP(const int32 DeltaXP);
	void AddLevel(const int32 DeltaLevel = 1);
	void AddAttributePoints(const int32 DeltaAttributePoints = 1);
	void AddSpellPoints(const int32 DeltaSpellPoints = 1);

protected:
	UFUNCTION()
	void OnRep_Level(const int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(const int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(const int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SpellPoints(const int32 OldSpellPoints);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	FP16OnPlayerStatChangedSignature OnXPChanged;
	FP16OnPlayerStatChangedSignature OnLevelChanged;
	FP16OnPlayerStatChangedSignature OnAttributePointsChanged;
	FP16OnPlayerStatChangedSignature OnSpellPointsChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UP16LevelUpInfoDataAsset> LevelUpInfos = nullptr;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
};
