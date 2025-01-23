// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P8PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UP8PlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT08_API IP8PlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void UnPause() = 0;
	virtual void GoToLobby() = 0;
};
