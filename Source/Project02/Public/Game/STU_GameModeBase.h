// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_CoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STU_GameModeBase.generated.h"

class ASTU_PlayerState;
class AAIController;

UCLASS()
class PROJECT02_API ASTU_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	FOnMatchStateChangeSignature OnMatchStateChange;
	
	ASTU_GameModeBase();
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
	virtual bool ClearPause() override;
	FGameData GetGameData() const;
	int32 GetCurrentRound() const;
	int32 GetRoundCountDown() const;
	void StopRoundWhenAllTeamDead();
	void Killed(const AController* KillerController, const AController* VictimController);
	void RespawnRequest(AController* RespawnController);

private:
	void SpawnTeams();
	void SetupTeammate(const AController* Controller, int32& TeamID);
	void StartRound();
	void GameTimerUpdate();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	FLinearColor GetColorByTeamID(const int32 TeamID, const TArray<FLinearColor>& Colors, const FLinearColor& Default) const;
	void SetPlayerColor(const AController* Controller) const;
	void LogPlayerInfo();
	void StartRespawn(const AController* RespawnController) const;
	void GameOver();
	void SetMatchState(ESTU_MatchState NewMatchState);
	void StopAllFire();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	TSubclassOf<AAIController> AIControllerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	TSubclassOf<APawn> AIPawnClass;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	FGameData GameData;

private:
	int32 CurrentRound   = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundHandle;
	ESTU_MatchState MatchState = ESTU_MatchState::WaitingToStart;
};
