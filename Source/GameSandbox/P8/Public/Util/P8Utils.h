#pragma once

#include "P8Utils.generated.h"

USTRUCT()
struct FP8ServerData
{
	GENERATED_BODY()

	FString SessionID;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
	FString ServerName;
};
