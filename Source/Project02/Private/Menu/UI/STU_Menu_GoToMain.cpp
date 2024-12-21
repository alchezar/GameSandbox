// Copyright (C) 2023, IKinder

#include "Menu/UI/STU_Menu_GoToMain.h"

#include "Components/Button.h"
#include "Game/STU_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void USTU_Menu_GoToMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMainMenu);
	}
}

void USTU_Menu_GoToMain::OnMainMenu()
{
	if (!GetWorld()) return;

	const USTU_GameInstance* GameInstance = GetWorld()->GetGameInstance<USTU_GameInstance>();
	if (!GameInstance) return;

	const FName MainLevelName = GameInstance->GetMainLevelName();
	if (MainLevelName.IsNone()) return;

	UGameplayStatics::OpenLevel(this, MainLevelName);
}
