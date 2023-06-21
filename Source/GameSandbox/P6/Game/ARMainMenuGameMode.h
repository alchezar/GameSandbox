// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARMainMenuGameMode.generated.h"

class UARMainMenu;

UCLASS()
class GAMESANDBOX_API AARMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UARMainMenu> MainMenuWidgetClass;
};
