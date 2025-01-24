// Copyright © 2024, IKinder

#include "UI/Inventory/P13PlayerStatsWidget.h"

#include "Game/P13GameState.h"
#include "Game/P13PlayerState.h"

void UP13PlayerStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerStateCached = GetOwningPlayerState<AP13PlayerState>();
	if (PlayerStateCached.IsValid())
	{
		PlayerStateCached->OnScoreChanged.AddUObject(this, &ThisClass::OnPlayerScoreChangedHandle);
		PlayerStateCached->OnLivesChanged.AddUObject(this, &ThisClass::OnPlayerLivesChangedHandle);
		OnPlayerLivesChangedHandle(PlayerStateCached->GetTotalLives());
		OnPlayerScoreChangedHandle(PlayerStateCached->GetTotalScore());

		SecondsFromStart = PlayerStateCached->GetTotalTime();
		OnOneSecondPassed();
		FTimerHandle EverySecondTimer;
		GetWorld()->GetTimerManager().SetTimer(EverySecondTimer, this, &ThisClass::OnOneSecondPassed, 1.f, true);
	}

	if (AP13GameState* GameState = GetWorld()->GetGameState<AP13GameState>())
	{
		GameState->OnPhaseChanged.AddUObject(this, &ThisClass::OnGamePhaseChangedHandle);
		OnGamePhaseChangedHandle(GameState->GetLastLevelPhase());
	}
}

void UP13PlayerStatsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UP13PlayerStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	/* Widget destructs only after pawn death, without the needed to refer to it's health component.
	 * Main reason is that OnHealthOver delegate calls UnPosses in pawn,
	 * which in turn calls this widget destruction, before delegate call and there is no guarantee of the correct calling order. */
	if (PlayerStateCached.IsValid())
	{
		PlayerStateCached->SaveTotalTime(SecondsFromStart);
	}
}

void UP13PlayerStatsWidget::OnPlayerScoreChangedHandle(const int32 NewScore)
{
	ScoreText->SetText(FText::AsNumber(NewScore));
}

void UP13PlayerStatsWidget::OnPlayerLivesChangedHandle(const int32 NewLives)
{
	LivesText->SetText(FText::AsNumber(NewLives - 1));
}

void UP13PlayerStatsWidget::OnOneSecondPassed()
{
	const int32 Min = SecondsFromStart / 60;
	const int32 Sec = SecondsFromStart % 60;

	const FString Time = NumberToTime(Min) + ":" + NumberToTime(Sec);
	TimeText->SetText(FText::FromString(Time));

	++SecondsFromStart;
}

void UP13PlayerStatsWidget::OnGamePhaseChangedHandle(const EP13LevelPhase LevelPhase)
{
	// const FName ValueAsName = UEnum::GetValueAsName(LevelPhase);
	const FText PhaseNum = FText::AsNumber(StaticCast<uint8>(LevelPhase) + 1);
	PhaseText->SetText(PhaseNum);
}

FString UP13PlayerStatsWidget::NumberToTime(const int32 Time)
{
	FString TimeString = FString::FromInt(Time);
	const bool bFull = TimeString.Len() > 1;

	return bFull ? TimeString : "0" + TimeString;
}
