// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13LobbyGameMode.generated.h"

class AP13LobbyPlayerController;
class UP13LobbyMenuWidget;

UCLASS()
class GAMESANDBOX_API AP13LobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP13LobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	/* ------------------------------- This -------------------------------- */
public:
	void UpdateSelectedLevel(const FText& InLevelName, const FName InLevelAddress);
	
	UFUNCTION(Server, Reliable)
	void Server_LaunchGame();

private:
	void SavePlayersColor();

	/* ----------------------------- Variables ----------------------------- */
private:
	FName LevelAddress;
	TArray<AP13LobbyPlayerController*> LobbyPlayerControllers;
};
