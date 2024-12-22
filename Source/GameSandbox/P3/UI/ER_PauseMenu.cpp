// Copyright (C) 2023, IKinder

#include "ER_PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "P3/Game/ER_GameModeBase.h"

void UER_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ContinueBtn || !MainBtn) return;

	ContinueBtn->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
	MainBtn->OnClicked.AddDynamic(this, &ThisClass::OnMainClicked);

	GameMode = Cast<AER_GameModeBase>(GetWorld()->GetAuthGameMode());
	check(GameMode);
}

void UER_PauseMenu::OnContinueClicked()
{
	// GetWorld()->GetAuthGameMode()->ClearPause();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UER_PauseMenu::OnMainClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameMode->GetMainLevelName());
	RemoveFromParent();
}
