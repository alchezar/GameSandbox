// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Projectile.generated.h"

UCLASS()
class GAMESANDBOX_API AP10Projectile : public AActor
{
	GENERATED_BODY()

public:
	AP10Projectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
