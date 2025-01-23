// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "P11SavePlayerInfo.generated.h"

UENUM()
enum class EP11PlayerSide : uint8
{
	None,
	Jedi,
	Syth
};

UCLASS()
class PROJECT11_API UP11SavePlayerInfo : public USaveGame
{
	GENERATED_BODY()

public:
	UP11SavePlayerInfo();
	
public:
	UPROPERTY()
	FText PlayerName;
	UPROPERTY()
	EP11PlayerSide PlayerSide;
	UPROPERTY()
	bool bShowFPS;
};
