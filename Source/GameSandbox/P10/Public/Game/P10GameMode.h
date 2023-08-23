// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P10GameMode.generated.h"

UCLASS()
class GAMESANDBOX_API AP10GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP10GameMode();
	void CompleteMission(APawn* InstigatorPawn, bool bSuccess);

protected:
	virtual void BeginPlay() override;
};
