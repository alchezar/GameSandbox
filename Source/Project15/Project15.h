// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

namespace P15
{
	constexpr float MinimalSpeed = 10.f;

	constexpr float WalkSpeed = 200.f;
	constexpr float RunSpeed  = 500.f;
}

#define EARLY_RETURN_IF(Condition)     \
if (Condition)                         \
{                                      \
	return;                            \
}

#define CONTINUE_IF(Condition)         \
if (Condition)                         \
{                                      \
	continue;                          \
}

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
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
