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
	
	AP13LobbyPlayerController* LobbyPlayerController = StaticCast<AP13LobbyPlayerController*>(NewPlayer);
	check(LobbyPlayerController)
	LobbyPlayerController->OnLogin();
	LobbyPlayerController->UpdateSelectedMapName(LevelName);
	LobbyPlayerControllers.Add(LobbyPlayerController);
}

void AP13LobbyGameMode::Server_LaunchGame_Implementation(const FName InLevelAddress)
{
	GetWorld()->ServerTravel(InLevelAddress.ToString());
}

void AP13LobbyGameMode::UpdateSelectedLevelForAll(const FString& InLevelName)
{
	LevelName = FText::FromString(InLevelName);
	
	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		LobbyController->UpdateSelectedMapName(LevelName);
	}
}

void AP13LobbyGameMode::UpdateSelectedColor(const FLinearColor OccupiedColor, const AP13LobbyPlayerController* Occupier)
{
	for (const AP13LobbyPlayerController* LobbyController : LobbyPlayerControllers)
	{
		LobbyController->UpdateSelectedColorOccupation(OccupiedColor, Occupier);
	}
}

void AP13LobbyGameMode::SavePlayersColor()
{
	
}
