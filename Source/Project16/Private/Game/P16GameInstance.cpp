// Copyright © 2025, Ivan Kinder

#include "Game/P16GameInstance.h"

#include "UI/ViewModel/P16MVVMLoadSlot.h"

void UP16GameInstance::UpdateGameParams(FString InSlotName, const int32 InSlotIndex, const FName InStartTag)
{
	LoadSlotName   = InSlotName;
	LoadSlotIndex  = InSlotIndex;
	PlayerStartTag = InStartTag;
}

void UP16GameInstance::UpdateGameParams(const UP16MVVMLoadSlot* InLoadSlot)
{
	UpdateGameParams(InLoadSlot->GetLoadSlotName(), InLoadSlot->GetSlotIndex(), InLoadSlot->PlayerStartTag);
}
