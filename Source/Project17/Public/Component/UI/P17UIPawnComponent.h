// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Component/P17ExtensionBaseComponent.h"
#include "P17UIPawnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP17OnPercentChangedDelegate, float, NewPercent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17UIPawnComponent : public UP17ExtensionBaseComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17UIPawnComponent();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP17OnPercentChangedDelegate OnHealthChanged;
};
