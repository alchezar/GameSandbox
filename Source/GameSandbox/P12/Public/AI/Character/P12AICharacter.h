// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12AICharacter.generated.h"

class UBehaviorTree;
class UP12AIPatrollingComponent;

UCLASS()
class GAMESANDBOX_API AP12AICharacter : public AP12BaseCharacter
{
	GENERATED_BODY()

public:
	explicit AP12AICharacter(const FObjectInitializer& ObjectInitializer);
	FORCEINLINE UP12AIPatrollingComponent* GetAIPatrollingComponent() const { return AIPatrollingComponent; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++")
	UP12AIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | AI")
	UBehaviorTree* BehaviorTree;
};
