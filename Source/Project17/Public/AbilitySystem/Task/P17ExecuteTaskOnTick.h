// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "P17ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP17OnExecuteTaskOnTickDelegate, float, DeltaTime);

UCLASS()
class PROJECT17_API UP17ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17ExecuteTaskOnTick();

public:
	/// @par UGameplayTask interface -------------------------------------------
	virtual void TickTask(const float DeltaTime) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++|", meta = (DisplayName = "ExecuteTaskOnTick", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UP17ExecuteTaskOnTick* CreateExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP17OnExecuteTaskOnTickDelegate OnTaskTick;
};
