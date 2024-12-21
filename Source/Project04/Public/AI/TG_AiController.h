// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TG_AiController.generated.h"

UCLASS()
class PROJECT04_API ATG_AiController : public AAIController
{
	GENERATED_BODY()

public:
	ATG_AiController();
	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;

public:
};
