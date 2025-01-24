// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P13Respond.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UP13Respond : public UInterface
{
	GENERATED_BODY()
};

class PROJECT13_API IP13Respond
{
	GENERATED_BODY()

public:
	virtual void Respond(AActor* Interactor, const bool bHead) = 0;
};
