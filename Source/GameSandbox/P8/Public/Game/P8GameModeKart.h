// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P8GameModeKart.generated.h"

UCLASS()
class GAMESANDBOX_API AP8GameModeKart : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP8GameModeKart();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
