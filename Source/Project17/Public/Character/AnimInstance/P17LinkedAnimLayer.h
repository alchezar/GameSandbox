// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17AnimInstanceBase.h"
#include "P17LinkedAnimLayer.generated.h"

class UP17AnimInstanceHero;

UCLASS()
class PROJECT17_API UP17LinkedAnimLayer : public UP17AnimInstanceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UP17AnimInstanceHero* GetHeroAnimInstance() const;
};
