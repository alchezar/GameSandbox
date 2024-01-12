// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13GameModeMenu.generated.h"

class UP13MainMenuWidget;

UCLASS()
class GAMESANDBOX_API AP13GameModeMenu : public AGameModeBase
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13GameModeMenu(){}
	
protected:
	virtual void BeginPlay() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
private:
	void ShowMainMenu();
	void SetMenuInputMode();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13MainMenuWidget> MainMenuWidgetClass;

private:
	UPROPERTY()
	UP13MainMenuWidget* MainMenuWidget = nullptr;
};
