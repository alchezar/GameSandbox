// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "P17AnimInstanceBase.generated.h"

UCLASS()
class PROJECT17_API UP17AnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool GetOwnerHaveTag(FGameplayTag TagToCheck) const;
};
