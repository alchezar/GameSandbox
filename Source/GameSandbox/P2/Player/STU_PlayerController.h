// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_CoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "STU_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class USTU_RespawnComponent;

UCLASS()
class GAMESANDBOX_API ASTU_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTU_PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void OnPausedGame();
	void OnMatchStateChanged(ESTU_MatchState State);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	USTU_RespawnComponent* RespawnComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* PauseAction;
};
