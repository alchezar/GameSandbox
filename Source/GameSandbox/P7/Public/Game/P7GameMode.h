// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P7GameMode.generated.h"

class AP7Character;

UCLASS()
class GAMESANDBOX_API AP7GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP7GameMode();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
