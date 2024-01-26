// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P13NetworkInterface.generated.h"

UINTERFACE()
class UP13NetworkInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP13NetworkInterface
{
	GENERATED_BODY()

public:
	virtual void CreateSession() = 0;
	virtual void JoinSession() = 0;
};
