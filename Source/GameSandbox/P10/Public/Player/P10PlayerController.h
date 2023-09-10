// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P10PlayerController.generated.h"

UCLASS()
class GAMESANDBOX_API AP10PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void OnMissionCompleted(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | EndGame")
	TSubclassOf<AActor> ViewPointClass;
};
