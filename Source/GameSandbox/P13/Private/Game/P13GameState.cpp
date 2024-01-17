// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameState.h"

#include "P13/Public/Controller/P13PlayerController.h"
#include "P13/Public/Library/P13Types.h"

AP13GameState::AP13GameState()
{
	
}

void AP13GameState::BeginPlay()
{
	Super::BeginPlay();
	SaveWinScores();
}

void AP13GameState::GoToNextPhase()
{
	LevelPhase = StaticCast<EP13LevelPhase>((StaticCast<uint8>(LevelPhase) + 1) % StaticCast<uint8>(EP13LevelPhase::MAX));
}

void AP13GameState::CheckWinCondition(const int32 TotalScore)
{
	const int32 CurrentIndex = StaticCast<uint8>(LevelPhase);
	if (!WinScores.IsValidIndex(CurrentIndex))
	{
		return;
	}

	const int32 WinScore = WinScores[CurrentIndex];
	if (WinScore > TotalScore)
	{
		return;
	}

	if (WinScores.IsValidIndex(CurrentIndex + 1))
	{
		GoToNextPhase();
		return;
	}

	AP13PlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AP13PlayerController>();
	if (!PlayerController->IsLocalController())
	{
		return;
	}
	PlayerController->OnGameWon();
}

void AP13GameState::SaveWinScores()
{
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	LevelPhase = StartLevelPhase;
	LevelSelectTable->GetAllRows<FP13LevelSelect>("", Levels);

	FP13LevelSelect** LevelResult = Levels.FindByPredicate([&](const FP13LevelSelect* Level)
	{
		return Level->RealName == *CurrentMapName;
	});
	if (!LevelResult || !(*LevelResult))
	{
		return;
	}
	
	WinScores = (*LevelResult)->PhaseWinScore;
}
