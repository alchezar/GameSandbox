// Copyright (C) 2023, IKinder

#include "UI/ServerList/P11ServerCreate.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Game/P11GameInstance.h"

// #include "Components/Button.h"

void UP11ServerCreate::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP11ServerCreate::OnBackHandle()
{
	OnCreateBackButtonPressed.Broadcast();
}

void UP11ServerCreate::OnCreateHandle()
{
	const int32 MaxPlayers = GetMaxPlayersNum();
	if (MaxPlayers == 0)
	{
		return;
	}
	if (ServerNameTextBox->GetText().IsEmpty())
	{
		ServerNameTextBox->SetError(FText::FromString("Empty"));
		return;
	}
	const bool bLAN = LanCheckBox->GetCheckedState() == ECheckBoxState::Checked;
	const FString LevelURL = LevelAddresses[ServerMapComboBox->GetSelectedIndex()];
	const FString ServerName = ServerNameTextBox->GetText().ToString();
	
	GameInstance->Host(MaxPlayers, bLAN, LevelURL, ServerName);
}