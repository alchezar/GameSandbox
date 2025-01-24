// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P13MenuPlayerController.generated.h"

class UP13StartMenuWidget;

UCLASS()
class PROJECT13_API AP13MenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13MenuPlayerController();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
private:
	void ShowMainMenu();
	void SetMenuInputMode() const;
	void SetGameInputMode() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13StartMenuWidget> MainMenuWidgetClass;
};
