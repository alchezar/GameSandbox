// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/P8Utils.h"
#include "P8MovementComponent.generated.h"

struct FP8Move;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT08_API UP8MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP8MovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE double GetMoveAlpha() const { return MoveAlpha; };
	FORCEINLINE double GetTurnAlpha() const { return TurnAlpha; };
	FORCEINLINE FVector GetVelocity() const { return Velocity; };
	FORCEINLINE FP8Move GetLastMove() const { return LastMove; };
	FORCEINLINE void SetVelocity(const FVector& NewVelocity) { Velocity = NewVelocity; };
	void SimulateMove(const FP8Move& TheMove);
	void Move(FVector2D Value);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "m/s"))
	float Speed = 20.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "kg"))
	float Mass = 1000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "N"))
	float MaxMoveForce = 10000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "m"))
	float MinTurnRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement") // kg/m
	float DragCoefficient = 16.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement") // kg/m
	float RollingCoefficient = 0.015f;

private:
	FVector Velocity = FVector::ZeroVector;
	float MoveAlpha = 0.f; // Throttle
	float TurnAlpha = 0.f; // SteeringThrow
	FP8Move LastMove;
};
