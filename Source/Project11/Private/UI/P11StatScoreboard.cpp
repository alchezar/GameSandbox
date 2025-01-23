// Copyright (C) 2023, IKinder

#include "UI/P11StatScoreboard.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P11PlayerState.h"
#include "UI/P11StatPlayerScoreWidget.h"

void UP11StatScoreboard::NativeConstruct()
{
	Super::NativeConstruct();
	ShowScore();
}

void UP11StatScoreboard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UP11StatScoreboard::ShowScore()
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
		AP11PlayerState* ThisPlayerState = Cast<AP11PlayerState>(PlayerState);
		if (!ThisPlayerState || !PlayerScoreClass)
		{
			return;
		}
		UP11StatPlayerScoreWidget* PlayerScoreWidget = CreateWidget<UP11StatPlayerScoreWidget>(this, PlayerScoreClass);
		if (!PlayerScoreWidget)
		{
			continue;
		}
		PlayerScoreWidget->SetPlayerState(ThisPlayerState);
		PlayerScoreWidget->AddToViewport();
		PlayersVBox->AddChild(PlayerScoreWidget);
	}
}
