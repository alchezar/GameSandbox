// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13GameModeMenu.generated.h"

class UP13MenuWidget;

UCLASS()
class GAMESANDBOX_API AP13GameModeMenu : public AGameModeBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameModeMenu() {}

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
private:
	void ShowMainMenu();
	void SetMenuInputMode();
	void SetGameInputMode();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13MenuWidget> MainMenuWidgetClass;

private:
	UPROPERTY()
	UP13MenuWidget* MainMenuWidget = nullptr;
};
