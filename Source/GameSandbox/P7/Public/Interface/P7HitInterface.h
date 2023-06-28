// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P7HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UP7HitInterface : public UInterface
{
	GENERATED_BODY()
};


class GAMESANDBOX_API IP7HitInterface
{
	GENERATED_BODY()

public:
	virtual void GetHit(const FVector& ImpactPoint) = 0;
};
