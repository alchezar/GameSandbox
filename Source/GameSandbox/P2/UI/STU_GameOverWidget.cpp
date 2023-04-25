// Copyright (C) 2023, IKinder

#include "STU_GameOverWidget.h"
#include "STU_GameModeBase.h"
#include "Components/VerticalBox.h"
#include "Player/STU_PlayerState.h"
#include "UI/STU_PlayerStatRowWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTU_GameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetWorld())
	{
		ASTU_GameModeBase* GameMode = Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &ThisClass::OnMatchStateChanged);
		}
	}

	ResetLevelButton->OnClicked.AddDynamic(this, &ThisClass::OnResetLevel);
}

void USTU_GameOverWidget::OnMatchStateChanged(const ESTU_MatchState State)
{
	if (State == ESTU_MatchState::GameOver)
	{
		UpdatePlayerStat();
	}
}

void USTU_GameOverWidget::UpdatePlayerStat()
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const AController* Controller = It->Get();
		if (!Controller) continue;
		
		const ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		const bool bPlayer     = Controller->IsPlayerController();
		const bool bFirstTeam  = PlayerState->GetTeamID() == 1;
		FString    TrooperName = PlayerState->GetPlayerName();
		if (!bPlayer)
		{
			TrooperName = bFirstTeam ? "Golden trooper" : "Sith trooper";
		}

		USTU_PlayerStatRowWidget* StatRowWidget = CreateWidget<USTU_PlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!StatRowWidget) continue;

		StatRowWidget->SetPlayerName(FText::FromString(TrooperName));
		StatRowWidget->SetKills(TextFromInt(PlayerState->GetKillsNum()));
		StatRowWidget->SetDeaths(TextFromInt(PlayerState->GetDeathNum()));
		StatRowWidget->SetTeams(TextFromInt(PlayerState->GetTeamID()));
		StatRowWidget->SetPlayerIndicatorVisibility(bPlayer);
		StatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

		bFirstTeam ? PlayerStatBox->AddChild(StatRowWidget) : EnemyStatBox->AddChild(StatRowWidget);
	}
}

FText USTU_GameOverWidget::TextFromInt(const int32 Num)
{
	return FText::FromString(FString::FromInt(Num));
}

void USTU_GameOverWidget::OnResetLevel()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	// FString LevelName = GetWorld()->GetMapName();
	UGameplayStatics::OpenLevel(this, FName(LevelName));
}
