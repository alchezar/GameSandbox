// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Task/P17ExecuteTaskOnTick.h"

UP17ExecuteTaskOnTick::UP17ExecuteTaskOnTick()
{
	bTickingTask = true;
}

void UP17ExecuteTaskOnTick::TickTask(const float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}

UP17ExecuteTaskOnTick* UP17ExecuteTaskOnTick::CreateExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UP17ExecuteTaskOnTick* ExecuteTaskOnTick = NewAbilityTask<UP17ExecuteTaskOnTick>(OwningAbility);
	return ExecuteTaskOnTick;
}
