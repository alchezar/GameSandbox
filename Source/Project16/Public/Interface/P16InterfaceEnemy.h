// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P16InterfaceEnemy.generated.h"

UINTERFACE()
class UP16InterfaceEnemy : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16InterfaceEnemy
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
