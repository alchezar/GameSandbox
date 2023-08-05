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

USTRUCT()
struct FP8ClientData
{
	GENERATED_BODY()

	FP8ClientData()
		: TimeSinceUpdate(0.f), TimeBetweenUpdates(0.f), StartTransform(FTransform::Identity), StartVelocity(FVector::ZeroVector) {}

	FP8ClientData(const float SinceUpdate, const float BetweenUpdates, const FTransform& Transform, const FVector& Velocity)
		: TimeSinceUpdate(SinceUpdate), TimeBetweenUpdates(BetweenUpdates), StartTransform(Transform), StartVelocity(Velocity) {}

	FORCEINLINE float GetRatio() const { return TimeSinceUpdate / TimeBetweenUpdates; }

	float TimeSinceUpdate;
	float TimeBetweenUpdates;
	FTransform StartTransform;
	FVector StartVelocity;
};

struct FP8HermiteCubicSpline
{
	FP8HermiteCubicSpline() : LocationStart(FVector::ZeroVector), LocationTarget(FVector::ZeroVector), DerivativeStart(FVector::ZeroVector), DerivativeTarget(FVector::ZeroVector), RotationStart(FQuat::Identity), RotationTarget(FQuat::Identity), LerpRatio(0.f), VelocityToDerivative(0.f) {}

	FP8HermiteCubicSpline(const FP8ClientData& ClientData, const FP8State& ServerState)
	{
		LerpRatio = ClientData.GetRatio();
		VelocityToDerivative = ClientData.TimeBetweenUpdates * 100.f;
		
		LocationStart = ClientData.StartTransform.GetLocation();
		LocationTarget = ServerState.Transform.GetLocation();
		
		DerivativeStart = ClientData.StartVelocity * VelocityToDerivative;
		DerivativeTarget = ServerState.Velocity * VelocityToDerivative;
		
		RotationStart = ClientData.StartTransform.GetRotation();
		RotationTarget = ServerState.Transform.GetRotation();
	}
	
	FVector InterpolateLocation() const
	{
		return FMath::CubicInterp(LocationStart, DerivativeStart, LocationTarget, DerivativeTarget, LerpRatio);
	}
	FVector InterpolateVelocity() const
	{
		return FMath::CubicInterpDerivative(LocationStart, DerivativeStart, LocationTarget, DerivativeTarget, LerpRatio)  / VelocityToDerivative;
	}

	FQuat InterpolateRotation() const
	{
		return FQuat::Slerp(RotationStart, RotationTarget, LerpRatio);
	}

	FVector LocationStart, LocationTarget, DerivativeStart, DerivativeTarget;
	FQuat RotationStart, RotationTarget;
	float LerpRatio, VelocityToDerivative;
};
