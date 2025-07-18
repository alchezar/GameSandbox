// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Util/P17Types.h"

class PROJECT17_API FP17CountDownAction : public FPendingLatentAction
{
	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	FP17CountDownAction(
		const float InTotalCountDownTime,
		const float InUpdateInterval,
		float& InOutRemainingTime,
		EP17CountDownOutput& InOutput,
		const FLatentActionInfo& InLatentInfo);

	virtual ~FP17CountDownAction() override;

	virtual void UpdateOperation(FLatentResponse& Response) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void CancelAction();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	float TotalCountDownTime = 0.f;
	float UpdateInterval = 0.f;
	float& OutRemainingTime;
	EP17CountDownOutput& Output;
	FName ExecutionFunction;
	int32 OutputLink = 0;
	FWeakObjectPtr CallbackTarget;

	float ElapsedInterval = 0.f;
	float ElapsedSinceStart = 0.f;
	bool bNeedToCancel = false;
};
