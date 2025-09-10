// Copyright © 2025, Ivan Kinder

#include "Util/Latent/P17CountDownAction.h"

FP17CountDownAction::FP17CountDownAction(
	const float InTotalCountDownTime,
	const float InUpdateInterval,
	float& InOutRemainingTime,
	EP17CountDownOutput& InOutput,
	const FLatentActionInfo& InLatentInfo)
	: TotalCountDownTime(InTotalCountDownTime)
	, UpdateInterval(InUpdateInterval)
	, OutRemainingTime(InOutRemainingTime)
	, Output(InOutput)
	, ExecutionFunction(InLatentInfo.ExecutionFunction)
	, OutputLink(InLatentInfo.Linkage)
	, CallbackTarget(InLatentInfo.CallbackTarget)
{}

FP17CountDownAction::~FP17CountDownAction()
{}

void FP17CountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		Output = EP17CountDownOutput::Canceled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	if (ElapsedSinceStart >= TotalCountDownTime)
	{
		Output = EP17CountDownOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		const float Interval = UpdateInterval > 0.f
			? ElapsedInterval
			: Response.ElapsedTime();
		ElapsedSinceStart += Interval;
		OutRemainingTime = FMath::Max(0.f, TotalCountDownTime - ElapsedSinceStart);
		Output = EP17CountDownOutput::Updated;
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		ElapsedInterval = 0.f;
	}
}

void FP17CountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
