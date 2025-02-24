// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "P16AbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP16AbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
