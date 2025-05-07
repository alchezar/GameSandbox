// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/P16GameMode.h"
#include "Game/P16SaveGame.h"
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
	FString GetLoadSlotName() const { return LoadSlotName; }
	int32   GetSlotIndex() const { return SlotIndex; }
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; };
	int32   GetPlayerLevel() const { return PlayerLevel; };

	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetSlotIndex(const int32 InSlotIndex);
	void SetPlayerName(const FString& InPlayerName);
	void SetMapName(const FString& InMapName);
	void SetPlayerLevel(const int32 InPlayerLevel);

	void InitSlot();
	void ResetSlot();
	void NewSlot(const AP16GameMode* GameMode, const FString& Name);;
	void LoadSlotFrom(const UP16SaveGame* SaveGame);

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
	UPROPERTY()
	FName PlayerStartTag = "Start";

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString LoadSlotName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	int32 SlotIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString PlayerName = "Default";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString MapName = "Default";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	int32 PlayerLevel = 1;
};
