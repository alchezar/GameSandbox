// Copyright © 2025, Ivan Kinder

#include "UI/ViewModel/P16MVVMLoadScreen.h"

#include "Project16.h"
#include "Game/P16GameInstance.h"
#include "Game/P16GameMode.h"
#include "Game/P16SaveGame.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"

UP16MVVMLoadSlot* UP16MVVMLoadScreen::GetLoadSlotViewModel(const int32 Index)
{
	return LoadSlots[Index];
}

void UP16MVVMLoadScreen::InitLoadSlots(const int32 Num)
{
	LoadSlots.Empty(Num);

	for (int32 Index = 0; Index < Num; Index++)
	{
		UP16MVVMLoadSlot* LoadSlot = NewObject<UP16MVVMLoadSlot>(this, LoadSlotViewModelClass);
		LoadSlot->SetLoadSlotName(FString::Printf(L"LoadSlot_%d", Index));
		LoadSlot->SetSlotIndex(Index);
		LoadSlots.Add(LoadSlot);
	}
}

void UP16MVVMLoadScreen::NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName)
{
	const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)

	LoadSlots[SlotIndex]->NewSlot(GameMode, EnteredName);

	UP16GameInstance* GameInstance = GameMode->GetGameInstance<UP16GameInstance>();
	EARLY_RETURN_IF(!GameInstance)
	GameInstance->UpdateGameParams(EnteredName, SlotIndex, GameMode->DefaultPlayerStartTag);
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

	SelectedSlot = LoadSlots[SlotIndex];
	OnSlotSelected.Broadcast();
}

void UP16MVVMLoadScreen::DeleteSlotButtonPressed()
{
	EARLY_RETURN_IF(!SelectedSlot)
	AP16GameMode::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->GetSlotIndex());
	SelectedSlot->ResetSlot();
	SelectedSlot = nullptr;
}

void UP16MVVMLoadScreen::StartButtonPressed()
{
	AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode || !SelectedSlot)
	UP16GameInstance* GameInstance = GameMode->GetGameInstance<UP16GameInstance>();
	EARLY_RETURN_IF(!GameInstance)
	GameInstance->UpdateGameParams(SelectedSlot);

	GameMode->TravelToMap(SelectedSlot);
	SelectedSlot = nullptr;
}

void UP16MVVMLoadScreen::LoadData()
{
	const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)

	for (int32 Index = 0; Index < LoadSlots.Num(); ++Index)
	{
		const UP16SaveGame* SaveGame = GameMode->GetSavedSlotData(LoadSlots[Index]->GetLoadSlotName(), Index);
		CONTINUE_IF(!SaveGame)

		LoadSlots[Index]->LoadSlotFrom(SaveGame);
	}
}
