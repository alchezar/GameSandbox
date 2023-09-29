// Copyright (C) 2023, IKinder

#include "P11/Public/UI/ServerList/P11ServerSingle.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Game/P11GameInstance.h"

void UP11ServerSingle::NativeConstruct()
{
	Super::NativeConstruct();
	GameInstance = GetWorld()->GetGameInstance<UP11GameInstance>();
	check(GameInstance)

	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackHandle);
	CreateBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateHandle);

	FindAllLevels();
}

void UP11ServerSingle::OnBackHandle()
{
	OnSingleBackBackButtonPressed.Broadcast();
}

void UP11ServerSingle::OnCreateHandle()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelAddresses[ServerMapComboBox->GetSelectedIndex()]));
	
	// const int32 MaxPlayers = GetMaxPlayersNum();
	// if (MaxPlayers == 0)
	// {
	// 	return;
	// }
	// const FString LevelURL = LevelAddresses[ServerMapComboBox->GetSelectedIndex()];
	// const int32 NameIndex = LevelURL.Find(".");
	// const FString LevelName = LevelURL.RightChop(NameIndex + 1);
	//
	// GameInstance->Host(MaxPlayers, true, LevelURL, LevelName);
}

int32 UP11ServerSingle::GetMaxPlayersNum() const
{
	const FString MaxPlayersString = MaxPlayersTextBox->GetText().ToString();
	const int32 MaxPlayers = FCString::Atoi(*MaxPlayersString);
	if (MaxPlayersString.IsEmpty())
	{
		MaxPlayersTextBox->SetError(FText::FromString("Empty!"));
		return 0;
	}
	if (MaxPlayers <= 0)
	{
		MaxPlayersTextBox->SetError(FText::FromString("Incorrect!"));
		return 0;
	}
	return MaxPlayers;
}

void UP11ServerSingle::FindAllLevels()
{
	TArray<FAssetData> OutAssetData = {};
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	AssetRegistryModule.Get().GetAssetsByPath(SearchPath, OutAssetData, true);
	for (FAssetData& Asset : OutAssetData)
	{
		if (!Cast<UWorld>(Asset.GetAsset()))
		{
			continue;
		}
		LevelAddresses.Add(Asset.GetObjectPathString());
		
		ServerMapComboBox->AddOption(Asset.AssetName.ToString());
		ServerMapComboBox->SetSelectedIndex(0);
	}
}
