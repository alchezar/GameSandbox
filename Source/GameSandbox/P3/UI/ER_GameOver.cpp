// Copyright (C) 2023, IKinder

#include "ER_GameOver.h"
#include "ER_MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/ER_GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UER_GameOver::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RestartBtn || !MainBtn) return;

	RestartBtn->OnClicked.AddDynamic(this, &ThisClass::OnRestartClicked);
	MainBtn->OnClicked.AddDynamic(this, &ThisClass::OnMainClicked);

	GameMode = Cast<AER_GameModeBase>(GetWorld()->GetAuthGameMode());
	check(GameMode);
}

void UER_GameOver::ShowFinalScore(const int32 Score)
{
	ScoreText->SetText(FText::AsNumber(Score));
}

void UER_GameOver::OnRestartClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameMode->GetGameLevelName());
	RemoveFromParent();
}

void UER_GameOver::OnMainClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameMode->GetMainLevelName());
	RemoveFromParent();
}
