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
	// Sets default values for this actor's properties
	ASTU_PlayerStart(const FObjectInitializer& ObjectInitializer);

	/*~ To take more control over PlayerStart selection, you can override the
	 * virtual AGameModeBase::FindPlayerStart and AGameModeBase::ChoosePlayerStart functions. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Start")
	int32 TeamID = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
