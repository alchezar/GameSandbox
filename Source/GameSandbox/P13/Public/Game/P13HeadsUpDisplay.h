// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P13HeadsUpDisplay.generated.h"

class UP13InGameWidget;

UCLASS()
class GAMESANDBOX_API AP13HeadsUpDisplay : public AHUD
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13HeadsUpDisplay();

protected:
	virtual void BeginPlay() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	void ShotInGame();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13InGameWidget> InGameWidgetClass;

private:
	TWeakObjectPtr<UP13InGameWidget> InGameWidgetCached;
};