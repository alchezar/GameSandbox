// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Util/P16Util.h"

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

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName);
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const int32 SlotIndex);
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(const int32 SlotIndex);

	void LoadData();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP16OnSlotSelectedSignature OnSlotSelected;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP16MVVMLoadSlot> LoadSlotViewModelClass = nullptr;

private:
	UPROPERTY()
	TArray<UP16MVVMLoadSlot*> LoadSlots = {};
};
