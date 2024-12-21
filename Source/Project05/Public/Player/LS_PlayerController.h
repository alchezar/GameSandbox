// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LS_PlayerController.generated.h"

UCLASS()
class PROJECT05_API ALS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALS_PlayerController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
