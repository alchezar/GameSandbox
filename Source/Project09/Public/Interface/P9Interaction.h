// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P9Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UP9Interaction : public UInterface
{
	GENERATED_BODY()
};

class PROJECT09_API IP9Interaction
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacter* Causer) = 0;
};
