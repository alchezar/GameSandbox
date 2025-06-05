// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "P16TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16MouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/// ----------------------------------------------------------------------------
/// @class   UP16TargetDataUnderMouse
/// @brief   Represents an ability task that returns the location of the mouse
/// @details Extends the base class with an ability to broadcast the target data
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void Activate() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Ability | Task", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UP16TargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

protected:
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag GameplayTag);

private:
	void SendMouseCursorData() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(BlueprintAssignable)
	FP16MouseTargetDataSignature ValidData;
};
