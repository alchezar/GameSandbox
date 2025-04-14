// Copyright © 2025, Ivan Kinder

#pragma once

#include "GameplayEffectTypes.h"
#include "Util/P16Type.h"
#include "P16GameplayEffectContext.generated.h"

/// ----------------------------------------------------------------------------
/// @class   FP16GameplayEffectContext
/// @brief   Represents a custom gameplay effect context.
/// @details Extends the base class with an ability to store the blocked hit and
/// critical hit flags.
/// ----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FP16GameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	virtual UScriptStruct* GetScriptStruct() const override;
	_NODISCARD
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	/// @brief Creates a copy of this context, used to duplicate for later
	/// modifications.
	/// @returns A copy of this context.
	_NODISCARD
	virtual FP16GameplayEffectContext* Duplicate() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	bool GetIsCriticalHit() const { return bCriticalHit; };
	_NODISCARD
	bool GetIsBlockedHit() const { return bBlockedHit; };
	_NODISCARD
	FP16DebuffSpec GetDebuffSpec() const { return DebuffSpec; };
	_NODISCARD
	FVector GetDeadImpulse() const { return DeathImpulse; };
	_NODISCARD
	FVector GetKnockbackForce() const { return KnockbackForce; };

	void SetIsCriticalHit(const bool bNewCriticalHit) { bCriticalHit = bNewCriticalHit; }
	void SetIsBlockedHit(const bool bNewBlockedHit) { bBlockedHit = bNewBlockedHit; }
	void SetDebuffSpec(const bool bSuccessful, const FP16DebuffInfo& InDebuffInfo, const FGameplayTag& InDamageType);
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType);
	void SetDeathImpulse(const FVector& NewImpulse) { DeathImpulse = NewImpulse; };
	void SetKnockbackForce(const FVector& NewForce) { KnockbackForce = NewForce; };

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY()
	bool bBlockedHit = false;
	UPROPERTY()
	bool bCriticalHit = false;

	TSharedPtr<FGameplayTag> DamageType = {};
	UPROPERTY()
	FP16DebuffSpec DebuffSpec = {};
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};

/// ----------------------------------------------------------------------------
/// @class TStructOpsTypeTraits
/// @brief Represents a specialization of TStructOpsTypeTraits for
/// FP16GameplayEffectContext.
/// @details Extends the base class with an ability to customize serialization
/// and copying for this class.
/// ----------------------------------------------------------------------------
template <>
struct TStructOpsTypeTraits<FP16GameplayEffectContext> : TStructOpsTypeTraitsBase2<FGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy          = true
	};
};
