// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13LobbyGameMode.generated.h"

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

	/* ------------------------------- This -------------------------------- */
private:
	void ShowLobbyMenu();
	void SetMenuInputMode() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13LobbyMenuWidget> LobbyMenuWidgetClass;

private:
	UPROPERTY()
	UP13LobbyMenuWidget* LobbyMenuWidget = nullptr;
};
