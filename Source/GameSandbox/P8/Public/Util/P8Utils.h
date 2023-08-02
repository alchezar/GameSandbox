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

USTRUCT()
struct FP8Move
{
	GENERATED_BODY()

	FP8Move() : MoveAlpha(0.f), TurnAlpha(0.f), DeltaTime(0.f), Time(0.f) {}

	FP8Move(const float Move, const float Turn, const float Delta, const float Started)
		: MoveAlpha(Move), TurnAlpha(Turn), DeltaTime(Delta), Time(Started) {}

	UPROPERTY()
	float MoveAlpha; // Throttle
	UPROPERTY()
	float TurnAlpha; // SteeringThrow
	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;
};

USTRUCT()
struct FP8State
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FP8Move LastMove;
};
