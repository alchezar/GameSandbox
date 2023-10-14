// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P12BaseCharacterMovementComponent.generated.h"

class AP12Ladder;

USTRUCT(BlueprintType)
struct FP12MaxSpeed
{
	GENERATED_BODY()

	FP12MaxSpeed() : Crouch(350.f), Walk(600.f), Run(900.f) {}

	UPROPERTY(EditAnywhere)
	float Crouch = 0.f;
	UPROPERTY(EditAnywhere)
	float Walk = 0.f;
	UPROPERTY(EditAnywhere)
	float Run = 0.f;
};

struct FP12MantleMovementParams
{
	FP12MantleMovementParams() {};
	FP12MantleMovementParams(const FVector& NewInitLocation, const FRotator& NewInitRotation, const FVector& NewTargetLocation, const FRotator& NewTargetRotation, const float NewDuration, const float NewStartTime, UCurveVector* NewCurve, const FVector& NewInitAnimLocation)
		: InitLocation(NewInitLocation), InitRotation(NewInitRotation), TargetLocation(NewTargetLocation), TargetRotation(NewTargetRotation), Duration(NewDuration), StartTime(NewStartTime), Curve(NewCurve), InitAnimationLocation(NewInitAnimLocation) {}
	
	FVector InitLocation = FVector::ZeroVector;
	FRotator InitRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;
	
	float Duration = 1.f;
	float StartTime = 0.f;

	UCurveVector* Curve = nullptr;

	FVector InitAnimationLocation = FVector::ZeroVector;
};

UENUM(BlueprintType)
enum class EP12CustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder   UMETA(DisplayName = "Ladder"),
	CMOVE_Max      UMETA(Hidden)
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12BaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UP12BaseCharacterMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual float GetMaxSpeed() const override;
	FORCEINLINE const AP12Ladder* GetCurrentLadder() const { return CurrentLadder; }
	void DefaultSetup();
	void SetPawnRotationMode(const bool bOrientToMovement);
	void ToggleMaxSpeed(const bool bRun);

	void Run(const bool bStart);
	
	void StartMantle(const FP12MantleMovementParams& MantleParams);
	void EndMantle();
	bool IsMantling();

	void AttachToLadder(const AP12Ladder* Ladder);
	void DetachFromLadder();
	bool IsOnLadder() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(const float DeltaTime, int32 Iterations) override;

private:
	void PhysMantling(const float DeltaTime, const int32 Iterations);
	void PhysLadder(const float DeltaTime, const int32 Iterations);

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP12MaxSpeed MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "C++ | Run", meta = (ClampMin = 1.f, UIMin = 1.f))
	float RunSpeed = 900.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Run", meta = (ClampMin = 1.f, UIMin = 1.f))
	float OutOfStaminaSpeed = 700.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Ladder")
	float LadderClimbingMaxSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Ladder")
	float LadderClimbingDeceleration = 2048.f;
	
private:
	bool bRunning = false;
	bool bOutOfStamina = false;
	bool bMantle = false;
	bool bLadder = false;
	FP12MantleMovementParams CurrentMantleParams;
	FTimerHandle MantlingTimer;
	UPROPERTY()
	const AP12Ladder* CurrentLadder = nullptr;
};
