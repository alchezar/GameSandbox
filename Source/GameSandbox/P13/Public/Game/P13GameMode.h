// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "P13GameMode.generated.h"

UCLASS()
class GAMESANDBOX_API AP13GameMode : public AGameMode
{
	GENERATED_BODY()
	
	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameMode();

protected:
	virtual void BeginPlay() override;
	
	/* ------------------------------- This -------------------------------- */
public:
	void Respawn(AController* NewPlayer);
	
	/* ----------------------------- Variables ----------------------------- */
protected:
	// UPROPERTY(EditAnywhere, Category = "C++")
	// TSubclassOf<UP13MenuWidget> MainMenuWidgetClass;
};
