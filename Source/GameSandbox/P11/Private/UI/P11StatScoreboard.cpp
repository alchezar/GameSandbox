// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11StatScoreboard.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Player/P11PlayerState.h"
#include "P11/Public/UI/P11StatPlayerScoreWidget.h"

void UP11StatScoreboard::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateScore();
	GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &ThisClass::UpdateScore, 1.f, true);
}

void UP11StatScoreboard::UpdateScore()
{
	PlayersCountText->SetText(FText::AsNumber(UGameplayStatics::GetNumPlayerStates(GetWorld())));
	MapNameText->SetText(FText::FromString(UGameplayStatics::GetCurrentLevelName(GetWorld())));
	PlayersVBox->ClearChildren();

	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	if (!GameState)
	{
		return;
	}
	for (auto PlayerState : GameState->PlayerArray)
	{
		const AP11PlayerState* ThisPlayerState = Cast<AP11PlayerState>(PlayerState);
		if (!ThisPlayerState || !PlayerScoreClass)
		{
			return;
		}
		UP11StatPlayerScoreWidget* PlayerScoreWidget = CreateWidget<UP11StatPlayerScoreWidget>(this, PlayerScoreClass);
		if (!PlayerScoreWidget)
		{
			continue;
		}
		PlayerScoreWidget->SetPlayerName(ThisPlayerState->GetThePlayerName());
		PlayerScoreWidget->SetIsDead(ThisPlayerState->GetIsDead());
		PlayerScoreWidget->SetKills(ThisPlayerState->GetKills());
		PlayerScoreWidget->SetDeaths(ThisPlayerState->GetDeaths());
		PlayerScoreWidget->SetPing(ThisPlayerState->GetPingInMilliseconds());

		PlayerScoreWidget->AddToViewport();
		PlayersVBox->AddChild(PlayerScoreWidget);
	}
	
}
