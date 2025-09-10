// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "P17BTSOrientToTarget.generated.h"

UCLASS()
class PROJECT17_API UP17BTSOrientToTarget : public UBTService
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
private:
	UP17BTSOrientToTarget();

	/// @par UBTNode -----------------------------------------------------------
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;

	/// @par UBTAuxiliaryNode --------------------------------------------------
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const float DeltaSeconds) override;

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
private:
	UPROPERTY(EditAnywhere, Category = "C++|Target")
	FBlackboardKeySelector InTargetActorKey;
	UPROPERTY(EditAnywhere, Category = "C++|Target")
	float RotationInternalSpeed = 5.f;
};
