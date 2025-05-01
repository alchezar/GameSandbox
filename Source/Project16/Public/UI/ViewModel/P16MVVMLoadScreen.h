// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "P16MVVMLoadScreen.generated.h"

class UP16MVVMLoadSlot;

UCLASS()
class PROJECT16_API UP16MVVMLoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintPure)
	UP16MVVMLoadSlot* GetLoadSlotViewModel(const int32 Index);

	void InitLoadSlots(const int32 Num = 3);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP16MVVMLoadSlot> LoadSlotViewModelClass = nullptr;

private:
	UPROPERTY()
	TArray<UP16MVVMLoadSlot*> LoadSlots = {};
};
