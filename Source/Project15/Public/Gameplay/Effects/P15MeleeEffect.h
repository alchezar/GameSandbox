// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "P15MeleeEffect.generated.h"

UCLASS()
class PROJECT15_API UP15MeleeEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UP15MeleeEffect();
};

UCLASS()
class PROJECT15_API UP15MeleeDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UP15MeleeDamageEffect();
};
