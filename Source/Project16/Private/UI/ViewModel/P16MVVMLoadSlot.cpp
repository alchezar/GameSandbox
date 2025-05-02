// Copyright © 2025, Ivan Kinder

#include "UI/ViewModel/P16MVVMLoadSlot.h"

void UP16MVVMLoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UP16MVVMLoadSlot::SetSlotIndex(const int32 InSlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotIndex, InSlotIndex);
}

void UP16MVVMLoadSlot::SetPlayerName(const FString& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UP16MVVMLoadSlot::InitSlot()
{
	OnSetWidgetSwitcher.Broadcast(StaticCast<int32>(SlotStatus));
}
