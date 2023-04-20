// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_CoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STU_GameModeBase.generated.h"

class AAIController;

UCLASS()
class GAMESANDBOX_API ASTU_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTU_GameModeBase();
	virtual void    StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// Getters
	FGameData GetGameData() const;
	int32     GetCurrentRound() const;
	int32     GetRoundCountDown() const;
	//
	void StopRoundWhenAllTeamDead();
	void Killed(const AController* KillerController, const AController* VictimController);
	void RespawnRequest(AController* RespawnController);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	TSubclassOf<AAIController> AIControllerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	TSubclassOf<APawn> AIPawnClass;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Game")
	FGameData GameData;

private:
	void         SpawnBots();
	void         StartRound();
	void         GameTimerUpdate();
	void         ResetPlayers();
	void         ResetOnePlayer(AController* Controller);
	void         CreateTeamsInfo() const;
	FLinearColor GetColorByTeamID(const int32 TeamID,  const TArray<FLinearColor>& Colors, const FLinearColor& Default) const;
	void         SetPlayerColor(const AController* Controller) const;
	void         LogPlayerInfo();
	void         StartRespawn(const AController* RespawnController) const;
	void         GameOver();

	int32        CurrentRound   = 1;
	int32        RoundCountDown = 0;
	FTimerHandle GameRoundHandle;
};
