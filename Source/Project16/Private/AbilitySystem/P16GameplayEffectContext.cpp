// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16GameplayEffectContext.h"

UScriptStruct* FP16GameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

bool FP16GameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// Almost identical to FGameplayEffectContext::NetSerialize, but with a few extra bits
	uint16 RepBits = 0;
	if (Ar.IsSaving())
	{
		// clang-format off
		RepBits |= (bReplicateInstigator && Instigator.IsValid())     << 0;
		RepBits |= (bReplicateEffectCauser && EffectCauser.IsValid()) << 1;
		RepBits |= (AbilityCDO.IsValid())                             << 2;
		RepBits |= (bReplicateSourceObject && SourceObject.IsValid()) << 3;
		RepBits |= (Actors.Num() > 0)                                 << 4;
		RepBits |= (HitResult.IsValid())                              << 5;
		RepBits |= (bHasWorldOrigin)                                  << 6;
		RepBits |= (bBlockedHit)                                      << 7;
		RepBits |= (bCriticalHit)                                     << 8;
		RepBits |= (DebuffSpec.bSuccessful)                           << 9;
		RepBits |= (DeathImpulse != FVector::ZeroVector)              << 10;
		RepBits |= (KnockbackForce != FVector::ZeroVector)            << 11;
		RepBits |= (RadialDamageParams.bRadial)                       << 12;
		// clang-format on
	}

	Ar.SerializeBits(&RepBits, 13);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading() && !HitResult.IsValid())
		{
			HitResult = MakeShared<FHitResult>();
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		DebuffSpec.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 10))
	{
		Ar << DeathImpulse;
	}
	if (RepBits & (1 << 11))
	{
		Ar << KnockbackForce;
	}
	if (RepBits & (1 << 12))
	{
		Ar << RadialDamageParams;
	}

	if (Ar.IsLoading())
	{
		// Just to initialize InstigatorAbilitySystemComponent
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}

	bOutSuccess = true;
	return true;
}

FP16GameplayEffectContext* FP16GameplayEffectContext::Duplicate() const
{
	FP16GameplayEffectContext* NewContext = new FP16GameplayEffectContext();
	*NewContext                           = *this;

	// Does a deep copy of the hit result
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}

	return NewContext;
}

void FP16GameplayEffectContext::SetDebuffSpec(const bool bSuccessful, const FP16DebuffInfo& InDebuffInfo, const FGameplayTag& InDamageType)
{
	// clang-format off
	DebuffSpec = FP16DebuffSpec {
		.bSuccessful = bSuccessful,
		.Damage      = InDebuffInfo.Damage,
		.Frequency   = InDebuffInfo.Frequency,
		.Duration    = InDebuffInfo.Duration,
		.DamageType  = MakeShared<FGameplayTag>(InDamageType)
	};
	// clang-format on
}

void FP16GameplayEffectContext::SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType)
{
	DebuffSpec.DamageType = InDamageType;
}
