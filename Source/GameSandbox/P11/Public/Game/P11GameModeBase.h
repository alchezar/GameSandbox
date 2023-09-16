// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P11GameModeBase.generated.h"

class AP11Character;

UCLASS()
class GAMESANDBOX_API AP11GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP11GameModeBase();
	void Respawn(AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP11Character> RespawnCharClass;
};
