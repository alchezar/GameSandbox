// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Type.h"
#include "P16Util.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnAttributeChangeSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnMessageWidgetRowSignature, FP16UIWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16AttributeInfoSignature, const FP16AttributeInfo&, AttributeInfo);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnEffectAppliedSignature, const FGameplayTagContainer& /*AssetTags*/)

UCLASS()
class UP16Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
