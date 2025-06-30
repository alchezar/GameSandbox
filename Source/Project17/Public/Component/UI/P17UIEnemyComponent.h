// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17UIPawnComponent.h"
#include "P17UIEnemyComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17UIEnemyComponent : public UP17UIPawnComponent
{
	GENERATED_BODY()

public:
	UP17UIEnemyComponent();
};
