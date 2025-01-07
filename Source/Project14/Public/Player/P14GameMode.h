// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Utils/P14Types.h"
#include "P14GameMode.generated.h"

UCLASS()
class PROJECT14_API AP14GameMode : public AGameModeBase
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14GameMode();

	virtual bool SetPause(APlayerController* PC, const FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	/* ------------------------------ Fields ------------------------------- */
public:
	UPROPERTY(BlueprintAssignable)
	FP14OnGamePause OnGamePause;
};
