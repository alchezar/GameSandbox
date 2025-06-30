// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17UIPawnComponent.h"
#include "P17UIHeroComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17UIHeroComponent : public UP17UIPawnComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17UIHeroComponent();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP17OnPercentChangedDelegate OnRageChanged;
};
