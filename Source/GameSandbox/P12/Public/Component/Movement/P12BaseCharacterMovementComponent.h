// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P12BaseCharacterMovementComponent.generated.h"

class AP12BaseCharacter;
class AP12Ladder;

USTRUCT(BlueprintType)
struct FP12MaxSpeed
{
	GENERATED_BODY()

	FP12MaxSpeed() {}

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float Crouch = 350.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float Walk = 600.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float Run = 900.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float OutOfStaminaRun = 700.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float LadderClimbing = 100.f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f, UIMin = 1.f))
	float Aim = 200.f;
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

UENUM(BlueprintType)
enum class EP12DetachFromLadderMethod : uint8
{
	Fall = 0,
	ReachingTheTop,
	ReachingTheBottom,
	JumpOff
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12BaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	friend class FP12SavedMove_BaseCharacter;

public:
	UP12BaseCharacterMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	FORCEINLINE const AP12Ladder* GetCurrentLadder() const { return CurrentLadder; }
	FORCEINLINE bool GetCanJump() const { return !bMantle && !bLadder; }
	FORCEINLINE bool GetCanMantle() const { return !bLadder; }
	FORCEINLINE bool GetCanWalk() const { return !bLadder; }
	void SetMantle(const bool bNewMantle) { bMantle = bNewMantle; }
	void DefaultSetup();
	void SetRotationMode(const bool bOrientToMovement);
	void ToggleMaxSpeed(const bool bRun);

	void StartMantle(const FP12MantleMovementParams& MantleParams);
	void EndMantle();
	bool IsMantling();

	void AttachToLadder(const AP12Ladder* Ladder);
	void DetachFromLadder(EP12DetachFromLadderMethod DetachFromLadderMethod = EP12DetachFromLadderMethod::Fall);
	bool IsOnLadder() const;
	float GetLadderSpeedRatio() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(const float DeltaTime, int32 Iterations) override;
	virtual void PhysicsRotation(const float DeltaTime) override;
	AP12BaseCharacter* GetBaseCharacterOwner() const;

private:
	void PhysMantling(const float DeltaTime, const int32 Iterations);
	void PhysLadder(const float DeltaTime, const int32 Iterations);
	float GetActorToLadderProjection(const FVector& Location) const;
	void SwitchForceRotation(const bool bEnable, const FRotator& TargetRotation = FRotator::ZeroRotator);
	UFUNCTION()
	void OnRep_IsMantling(bool bWasMantling);

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP12MaxSpeed MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "C++ | Ladder")
	float LadderClimbingDeceleration = 2048.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Ladder")
	float LadderToCharacterOffset = 55.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Ladder")
	float JumpOffFromLadderSpeed = 500.f;

private:
	bool bRunning = false;
	bool bOutOfStamina = false;
	UPROPERTY(ReplicatedUsing = "OnRep_IsMantling")
	bool bMantle = false;
	bool bLadder = false;
	FP12MantleMovementParams CurrentMantleParams;
	FTimerHandle MantlingTimer;
	UPROPERTY()
	const AP12Ladder* CurrentLadder = nullptr;
	FRotator ForceTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;
};

class FP12SavedMove_BaseCharacter : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

public:
	FP12SavedMove_BaseCharacter();
	virtual void Clear() override;
	virtual uint8 GetCompressedFlags() const override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* InCharacter, float MaxDelta) const override;
	virtual void SetMoveFor(ACharacter* InCharacter, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PrepMoveFor(ACharacter* Character) override;	
	
private:
	uint8 bSavedIsRunning : 1;
	uint8 bSavedIsMantling : 1;
};

class FP12NetworkPredictionData_Client_BaseCharacter : public FNetworkPredictionData_Client_Character
{
	typedef FNetworkPredictionData_Client_Character Super;

public:
	explicit FP12NetworkPredictionData_Client_BaseCharacter(const UCharacterMovementComponent& ClientMovement);
	virtual FSavedMovePtr AllocateNewMove() override;
};

