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
	FORCEINLINE TSoftObjectPtr<UWorld> GetStartupMap() const { return StartupMap; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSoftObjectPtr<UWorld> StartupMap;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP11Character> RespawnCharClass;
};
