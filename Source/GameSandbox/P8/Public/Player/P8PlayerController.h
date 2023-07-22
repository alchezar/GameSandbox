// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P8PlayerController.generated.h"

UCLASS()
class GAMESANDBOX_API AP8PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AP8PlayerController();

protected:
	virtual void BeginPlay() override;
};
