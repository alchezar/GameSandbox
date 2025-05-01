// Copyright © 2025, Ivan Kinder

#include "UI/ViewModel/P16MVVMLoadSlot.h"

void UP16MVVMLoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
