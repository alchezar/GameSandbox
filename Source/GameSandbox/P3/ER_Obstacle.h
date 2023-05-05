// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_Obstacle.generated.h"

UCLASS()
class GAMESANDBOX_API AER_Obstacle : public AActor
{
	GENERATED_BODY()

public:
	AER_Obstacle();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
};
