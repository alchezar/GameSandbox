// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_GameDataWidget.generated.h"

class ASTU_PlayerState;
class ASTU_GameModeBase;

UCLASS()
class GAMESANDBOX_API USTU_GameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	int32 GetKillsNum() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	int32 GetDeathsNum() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	int32 GetCurrentRoundNum() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	int32 GetTotalRoundNum() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | UI")
	int32 GetRoundSecondsRemaining() const;

private:
	ASTU_GameModeBase* GetGameMode() const;
	ASTU_PlayerState*  GetPlayerState() const;
};
