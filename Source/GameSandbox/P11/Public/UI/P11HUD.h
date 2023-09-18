// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P11HUD.generated.h"

class UP11StatScoreboard;
class UP11MenuFPS;
class UP11MainMenu;
class UP11MainWidget;

UCLASS()
class GAMESANDBOX_API AP11HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP11HUD();
	void ShowUI(const bool bVisible);
	void ShowMainMenu(const bool bVisible);
	void ShowFPS();
	void ShowScore(const bool bVisible);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP11MainWidget> MainWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP11MainMenu> MainMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP11MenuFPS> MenuFpsClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP11StatScoreboard> ScoreboardClass;
	
private:
	UPROPERTY()
	UP11MainWidget* MainWidget = nullptr;
	UPROPERTY()
	UP11MainMenu* MainMenu = nullptr;
	UPROPERTY()
	UP11MenuFPS* MenuFPS = nullptr;
	UPROPERTY()
	UP11StatScoreboard* Scoreboard = nullptr;
};
