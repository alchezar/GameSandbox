// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P8MenuInterface.generated.h"

UINTERFACE()
class UP8MenuInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP8MenuInterface
{
	GENERATED_BODY()

public:
	virtual void Host() = 0;
	virtual void Join(uint32 Index) = 0;
	virtual void RefreshServerList() = 0;
};
