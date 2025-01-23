// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P8GameMode.generated.h"

UCLASS()
class PROJECT08_API AP8GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP8GameMode();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
