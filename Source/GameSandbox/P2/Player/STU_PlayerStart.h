// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "STU_PlayerStart.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_PlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ASTU_PlayerStart(const FObjectInitializer& ObjectInitializer);

	/*~ To take more control over PlayerStart selection, you can override the
	 * virtual AGameModeBase::FindPlayerStart and AGameModeBase::ChoosePlayerStart functions. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Start")
	int32 TeamID = 0;

protected:
	virtual void BeginPlay() override;
};
