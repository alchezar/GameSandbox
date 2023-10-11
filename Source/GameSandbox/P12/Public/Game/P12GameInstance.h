// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P12GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP12GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/* Allow to Exec console commands from GameInstance. */
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
};
