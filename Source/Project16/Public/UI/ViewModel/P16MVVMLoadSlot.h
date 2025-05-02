// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Util/P16Util.h"
#include "P16MVVMLoadSlot.generated.h"

UCLASS()
class PROJECT16_API UP16MVVMLoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FString GetLoadSlotName() const { return LoadSlotName; };
	int32   GetSlotIndex() const { return SlotIndex; };
	FString GetPlayerName() const { return PlayerName; };

	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetSlotIndex(const int32 InSlotIndex);
	void SetPlayerName(const FString& InPlayerName);

	void InitSlot();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP16OnSetWidgetSwitcherSignature OnSetWidgetSwitcher;
	UPROPERTY(BlueprintAssignable)
	FP16OnEnableSelectSlotButtonSignature OnEnableSelectSlotButton;
	UPROPERTY()
	EP16SaveSlotStatus SlotStatus = EP16SaveSlotStatus::Vacant;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString LoadSlotName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	int32 SlotIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString PlayerName = "Default";
};
