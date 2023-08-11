// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P9GameMode.generated.h"

class AP9PunchCharacter;
class AP9HUD;

UCLASS()
class GAMESANDBOX_API AP9GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP9GameMode();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP9HUD> DefaultHUDClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP9PunchCharacter> DefaultCharacterClass;
};
