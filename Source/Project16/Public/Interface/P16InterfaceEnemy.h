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
	virtual void ToggleHighlight(const bool bOn) = 0;
};
