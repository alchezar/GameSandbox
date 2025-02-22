// Copyright © 2024, IKinder

#include "Game/P13GameState.h"

#include "Controller/P13PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Library/P13Types.h"

AP13GameState::AP13GameState() {}

void AP13GameState::BeginPlay()
{
	Super::BeginPlay();

	LevelPhase = StartLevelPhase;
	OnPhaseChanged.Broadcast(LevelPhase);

	SaveWinScores();
}

int32 AP13GameState::GetMaxEnemiesOnPhase()
{
	const int32 PhaseNum = StaticCast<uint8>(LevelPhase);
	check(MaxEnemies.IsValidIndex(PhaseNum))

	return MaxEnemies[PhaseNum];
}

void AP13GameState::GoToNextPhase()
{
	LevelPhase = StaticCast<EP13LevelPhase>((StaticCast<uint8>(LevelPhase) + 1) % StaticCast<uint8>(EP13LevelPhase::MAX));
	OnPhaseChanged.Broadcast(LevelPhase);
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

	for (const TObjectPtr<APlayerState> Player : PlayerArray)
	{
		AP13PlayerController* OwnerController = Cast<AP13PlayerController>(Player->GetPlayerController());
		if (!OwnerController || !OwnerController->IsLocalController())
		{
			continue;
		}
		OwnerController->OnGameWon();
	}
}

void AP13GameState::SaveWinScores()
{
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

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
	MaxEnemies = (*LevelResult)->MaxEnemiesPerPhase;
}
