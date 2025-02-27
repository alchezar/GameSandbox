// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Util.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnHealthChangeSignature, float, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnMaxHealthChangeSignature, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnManaChangeSignature, float, NewMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnMaxManaChangeSignature, float, NewMaxMana);

UCLASS()
class UP16Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
