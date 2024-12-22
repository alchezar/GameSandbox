// Copyright (C) 2023, IKinder

#include "STU_GameHUD.h"

#include "STU_BaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "P2/STU_GameModeBase.h"

ASTU_GameHUD::ASTU_GameHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_GameHUD::DrawHUD()
{
	Super::DrawHUD();
	// DrawCrossHair();
}

void ASTU_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTU_MatchState::InProgress, CreateWidget<USTU_BaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTU_MatchState::Pause, CreateWidget<USTU_BaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESTU_MatchState::GameOver, CreateWidget<USTU_BaseWidget>(GetWorld(), GameOverWidgetClass));
	for (auto [MatchState, GameWidget] : GameWidgets)
	{
		if (!GameWidget) continue;
		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GetWorld())
	{
		ASTU_GameModeBase* GameMode = Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &ThisClass::OnMatchStateChangedHandle);
		}
	}
}

void ASTU_GameHUD::DrawCrossHair()
{
	constexpr float        HalfLineSize  = 10.f;
	constexpr float        LineThickness = 2.f;
	const TInterval<float> Center(Canvas->SizeX / 2, Canvas->SizeY / 2);
	const FLinearColor     LineColor = FLinearColor::Red;
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTU_GameHUD::OnMatchStateChangedHandle(const ESTU_MatchState NewState)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameWidgets.Contains(NewState))
	{
		CurrentWidget = GameWidgets[NewState];
	}
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->PlayShow();
	}
	// CurrentWidget ? CurrentWidget->SetVisibility(ESlateVisibility::Visible) : ' ';	
}
