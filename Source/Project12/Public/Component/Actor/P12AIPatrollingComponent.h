// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12AIPatrollingComponent.generated.h"

class AP12PatrollingPath;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT12_API UP12AIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FVector SelectClosestWayPoint();
	FVector SelectNextWayPoint();
	bool GetCanPatrol() const;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C++")
	AP12PatrollingPath* PatrollingPath;

private:
	int32 CurrentWayPointIndex = INDEX_NONE;
};
