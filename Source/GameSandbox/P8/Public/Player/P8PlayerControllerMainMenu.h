// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P8PlayerControllerMainMenu.generated.h"

UCLASS()
class GAMESANDBOX_API AP8PlayerControllerMainMenu : public APlayerController
{
	GENERATED_BODY()

public:
	AP8PlayerControllerMainMenu();

protected:
	virtual void BeginPlay() override;
};
