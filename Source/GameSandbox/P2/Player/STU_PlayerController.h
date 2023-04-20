// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STU_PlayerController.generated.h"

class USTU_RespawnComponent;

UCLASS()
class GAMESANDBOX_API ASTU_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTU_PlayerController();

protected:
	// virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	USTU_RespawnComponent* RespawnComponent;
};
