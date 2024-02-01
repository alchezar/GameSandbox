// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13LobbyGameMode.generated.h"

class AP13CharacterBase;
class ASkeletalMeshActor;
class ATargetPoint;
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
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	/* ------------------------------- This -------------------------------- */
public:
	UFUNCTION(Server, Reliable)
	void Server_LaunchGame(const FName InLevelAddress);
	void UpdateSelectedLevelForAll(const FString& InLevelName);
	void UpdateSelectedColor(const FLinearColor OccupiedColor, const AP13LobbyPlayerController* Occupier);

private:
	void SavePlayersColor();
	void InitSpawnPoints();
	ATargetPoint* GetFreeSpawnPoint();
	void SpawnPlayerPreview(AP13LobbyPlayerController* PreviewOwner);
	AActor* FindCameraView() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<AP13CharacterBase> CharBaseClass;
		
private:
	FText LevelName;
	TArray<AP13LobbyPlayerController*> LobbyPlayerControllers;
	TMap<ATargetPoint*, bool> SpawnPointsMap;
	FLinearColor DefaultPawnColor = FLinearColor::White;
};
