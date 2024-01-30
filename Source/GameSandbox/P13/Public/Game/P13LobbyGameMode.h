// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13LobbyGameMode.generated.h"

class AP13LobbyPlayerController;

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
	UFUNCTION(Server, Reliable)
	void Server_LaunchGame(const FName InLevelAddress);
	void UpdateSelectedLevelForAll(const FString& InLevelName);
	void UpdateSelectedColor(const FLinearColor OccupiedColor, const AP13LobbyPlayerController* Occupier);

private:
	void SavePlayersColor();

	/* ----------------------------- Variables ----------------------------- */
private:
	FText LevelName;
	TArray<AP13LobbyPlayerController*> LobbyPlayerControllers;
};
