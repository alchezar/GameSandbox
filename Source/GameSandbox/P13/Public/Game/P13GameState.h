// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "P13/Public/Library/P13Types.h"
#include "P13GameState.generated.h"

struct FP13LevelSelect;

DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnPhaseChangedSignature, EP13LevelPhase /*Phase*/)

UCLASS()
class GAMESANDBOX_API AP13GameState : public AGameState
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameState();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE EP13LevelPhase GetLastLevelPhase() const { return LevelPhase; }
	int32 GetMaxEnemiesOnPhase();
	void GoToNextPhase();
	void CheckWinCondition(const int32 TotalScore);

private:
	void SaveWinScores();

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnPhaseChangedSignature OnPhaseChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	EP13LevelPhase StartLevelPhase = EP13LevelPhase::Phase1;

	UPROPERTY(EditAnywhere, Category = "C++")
	UDataTable* LevelSelectTable = nullptr;

private:
	EP13LevelPhase LevelPhase = EP13LevelPhase::Phase1;
	TArray<FP13LevelSelect*> Levels;
	TArray<int32> WinScores = {};
	TArray<int32> MaxEnemies = {};
};
