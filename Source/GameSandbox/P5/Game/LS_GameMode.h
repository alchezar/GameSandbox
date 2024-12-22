// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LS_GameMode.generated.h"

UCLASS()
class GAMESANDBOX_API ALS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALS_GameMode();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
