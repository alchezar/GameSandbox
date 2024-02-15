// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P13CombatInterface.generated.h"

UINTERFACE(BlueprintType)
class UP13CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP13CombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++")
	bool GetCanAttack() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++")
	bool GetCanMove() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++")
	bool AttackAttempt(const FVector& TargetLocation);
};
