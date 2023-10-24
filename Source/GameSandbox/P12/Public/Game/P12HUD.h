// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P12HUD.generated.h"

class UP12PlayerHUDWidget;

UCLASS()
class GAMESANDBOX_API AP12HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP12HUD();

protected:
	virtual void BeginPlay() override;

private:
	void ShowGameScreen();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP12PlayerHUDWidget> GameScreenClass;

};
