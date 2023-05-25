// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TG_PlayerController.generated.h"

UCLASS()
class GAMESANDBOX_API ATG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATG_PlayerController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
