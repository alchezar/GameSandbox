// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "P16DamageTextComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16DamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(const float Damage);
};
