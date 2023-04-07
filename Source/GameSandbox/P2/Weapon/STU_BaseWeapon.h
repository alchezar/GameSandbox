// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_BaseWeapon.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
