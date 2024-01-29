// Copyright © 2024, IKinder

#include "P13/Public/Game/P13LobbyGameMode.h"

#include "P13/Public/Controller/P13LobbyPlayerController.h"

AP13LobbyGameMode::AP13LobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseSeamlessTravel = true;
}

void AP13LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AP13LobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (!NewPlayer->IsA<AP13LobbyPlayerController>())
	{
		return;
	}
	LobbyPlayerControllers.Add(StaticCast<AP13LobbyPlayerController*>(NewPlayer));
}

void AP13LobbyGameMode::UpdateSelectedLevel(const FText& InLevelName, const FName InLevelAddress)
{
	LevelAddress = InLevelAddress;
	
	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		LobbyController->UpdateSelectedMapName(InLevelName);
	}
}

void AP13LobbyGameMode::Server_LaunchGame_Implementation()
{
	
}

void AP13LobbyGameMode::SavePlayersColor()
{
	
}
