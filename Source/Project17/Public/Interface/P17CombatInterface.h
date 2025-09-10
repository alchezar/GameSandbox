// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P17CombatInterface.generated.h"

class UP17CombatPawnComponent;

UINTERFACE()
class UP17CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT17_API IP17CombatInterface
{
	GENERATED_BODY()

public:
	virtual UP17CombatPawnComponent* GetCombatComponent() const = 0;
};
