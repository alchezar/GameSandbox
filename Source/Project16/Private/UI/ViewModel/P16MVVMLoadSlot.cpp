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

void UP16MVVMLoadSlot::SetMapName(const FString& InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UP16MVVMLoadSlot::SetPlayerLevel(const int32 InPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
}

void UP16MVVMLoadSlot::InitSlot()
{
	OnSetWidgetSwitcher.Broadcast(StaticCast<int32>(SlotStatus));
}

void UP16MVVMLoadSlot::ResetSlot()
{
	SetPlayerName("Default");
	SlotStatus = EP16SaveSlotStatus::Vacant;
	InitSlot();
	OnEnableSelectSlotButton.Broadcast(true);
}

void UP16MVVMLoadSlot::NewSlot(const AP16GameMode* GameMode, const FString& Name)
{
	SetMapName(GameMode->GetDefaultMapName());
	SetPlayerName(Name);
	SlotStatus     = EP16SaveSlotStatus::Taken;
	PlayerStartTag = GameMode->DefaultPlayerStartTag;
	InitSlot();

	GameMode->SaveSlotData(this, SlotIndex);
}

void UP16MVVMLoadSlot::LoadSlotFrom(const UP16LoadScreenSaveGame* SaveGame)
{
	SetPlayerName(SaveGame->PlayerName);
	SetMapName(SaveGame->MapName);
	SlotStatus     = SaveGame->SlotStatus;
	PlayerStartTag = SaveGame->PlayerStartTag;
	InitSlot();
}
