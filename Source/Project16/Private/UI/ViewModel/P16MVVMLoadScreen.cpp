// Copyright © 2025, Ivan Kinder

#include "UI/ViewModel/P16MVVMLoadScreen.h"

#include "Project16.h"
#include "Game/P16GameMode.h"
#include "Game/P16LoadScreenSaveGame.h"
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

void UP16MVVMLoadScreen::NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName)
{
	AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)

	LoadSlots[SlotIndex]->SetPlayerName(EnteredName);
	LoadSlots[SlotIndex]->SlotStatus = EP16SaveSlotStatus::Taken;

	GameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitSlot();
}

void UP16MVVMLoadScreen::NewGameButtonPressed(const int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SlotStatus = EP16SaveSlotStatus::Named;
	LoadSlots[SlotIndex]->InitSlot();
}

void UP16MVVMLoadScreen::SelectSlotButtonPressed(const int32 SlotIndex)
{
	for (int32 Index = 0; Index < LoadSlots.Num(); ++Index)
	{
		const bool bEnable = Index != SlotIndex;
		LoadSlots[Index]->OnEnableSelectSlotButton.Broadcast(bEnable);
	}

	OnSlotSelected.Broadcast();
}

void UP16MVVMLoadScreen::LoadData()
{
	const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)

	for (int32 Index = 0; Index < LoadSlots.Num(); ++Index)
	{
		const UP16LoadScreenSaveGame* SaveGame = GameMode->GetSavedSlotData(LoadSlots[Index]->GetLoadSlotName(), Index);
		CONTINUE_IF(!SaveGame)

		LoadSlots[Index]->SetPlayerName(SaveGame->PlayerName);
		LoadSlots[Index]->SlotStatus = SaveGame->SlotStatus;

		LoadSlots[Index]->InitSlot();
	}
}
