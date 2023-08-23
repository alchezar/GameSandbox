// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P10GameState.generated.h"

UCLASS()
class GAMESANDBOX_API AP10GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnMissionComplete(APawn* InstigatorPawn, bool bSuccess);
};
