// Copyright © 2025, Ivan Kinder

#include "UI/ViewModel/P16MVVMLoadScreen.h"

#include "UI/ViewModel/P16MVVMLoadSlot.h"

UP16MVVMLoadSlot* UP16MVVMLoadScreen::GetLoadSlotViewModel(const int32 Index)
{
	return LoadSlots[Index];
}

void UP16MVVMLoadScreen::InitLoadSlots(const int32 Num)
{
	LoadSlots.Empty(Num);

	for (int32 i = 0; i < Num; i++)
	{
		UP16MVVMLoadSlot* LoadSlot = NewObject<UP16MVVMLoadSlot>(this, LoadSlotViewModelClass);
		LoadSlot->SetLoadSlotName(FString::Printf(L"LoadSlot_%d", i));
		LoadSlots.Add(LoadSlot);
	}
}
