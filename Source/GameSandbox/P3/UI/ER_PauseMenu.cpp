// Copyright (C) 2023, IKinder

#include "ER_PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ER_GameModeBase.h"

void UER_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RestartBtn || !ExitBtn || !ContinueBtn) return;

	ContinueBtn->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
	RestartBtn->OnClicked.AddDynamic(this, &ThisClass::OnRestartClicked);
	ExitBtn->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);
}

void UER_PauseMenu::OnContinueClicked()
{
	// GetWorld()->GetAuthGameMode()->ClearPause();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UER_PauseMenu::OnRestartClicked()
{
	const AER_GameModeBase* GameMode = Cast<AER_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	UGameplayStatics::OpenLevel(GetWorld(), GameMode->GetLevelName());
	RemoveFromParent();
}

void UER_PauseMenu::OnExitClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
