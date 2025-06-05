// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P16AIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class PROJECT16_API AP16AIController : public AAIController
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16AIController();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTree = nullptr;
};
