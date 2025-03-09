// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CombatInterface.generated.h"

UINTERFACE()
class UP16CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16CombatInterface
{
	GENERATED_BODY()

public:
	virtual int32   GetPlayerLevel() { return 0; }
	virtual FVector GetCombatSocketLocation() { return FVector::ZeroVector; }
};
