// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARAIController.generated.h"

class UBehaviorTree;

UCLASS()
class GAMESANDBOX_API AARAIController : public AAIController
{
	GENERATED_BODY()

public:
	AARAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	UBehaviorTree* BehaviorTree;
	

};
