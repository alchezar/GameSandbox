// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12PatrollingPath.generated.h"

UCLASS()
class PROJECT12_API AP12PatrollingPath : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE const TArray<FVector>& GetWayPoints() const { return WayPoints; }

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C++", meta = (MakeEditWidget))
	TArray<FVector> WayPoints;
};
