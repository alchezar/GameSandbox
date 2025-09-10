// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P17UIInterface.generated.h"

class UP17UIEnemyComponent;
class UP17UIHeroComponent;
class UP17UIPawnComponent;

UINTERFACE()
class UP17UIInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT17_API IP17UIInterface
{
	GENERATED_BODY()

public:
	virtual UP17UIPawnComponent* GetPawnUIComponent() const = 0;
	virtual UP17UIHeroComponent* GetHeroUIComponent() const { return nullptr; };
	virtual UP17UIEnemyComponent* GetEnemyUIComponent() const { return nullptr; };
};
