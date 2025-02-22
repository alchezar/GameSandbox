// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P7WallRunComponent.generated.h"

class UCharacterMovementComponent;
class AP7Character;

UENUM(BlueprintType)
enum EWallSide : uint8
{
	EWS_Right UMETA(DisplayName = "Right"),
	EWS_Left  UMETA(DisplayName = "Left")
};

USTRUCT()
struct FWallRunTraceInfo
{
	GENERATED_BODY()

	float InitialTraceLength;
	float StartOffsetBackwards;
	float StartOffsetAwayFromWall;
	float EndOffsetBackwards;
	float EndOffsetAwayFromWall;

	FWallRunTraceInfo()
		: InitialTraceLength(0.f), StartOffsetBackwards(0.f), StartOffsetAwayFromWall(0.f), EndOffsetBackwards(0.f), EndOffsetAwayFromWall(0.f) {};
	
	explicit FWallRunTraceInfo(const float NewInitialTraceLength, const float NewStartOffsetBackwards, const float NewStartOffsetAwayFromWall, const float NewEndOffsetBackwards, const float NewEndOffsetAwayFromWall)
	: InitialTraceLength(NewInitialTraceLength), StartOffsetBackwards(NewStartOffsetBackwards), StartOffsetAwayFromWall(NewStartOffsetAwayFromWall), EndOffsetBackwards(NewEndOffsetBackwards), EndOffsetAwayFromWall(NewEndOffsetAwayFromWall) {};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP7WallRunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP7WallRunComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE bool GetIsWallRunning()           const { return bWallRunning; }
	FORCEINLINE bool GetIsWallJumping()           const { return bWallJumping; }
	FORCEINLINE EWallSide GetWallSide()           const { return WallSide; }
	FORCEINLINE float GetArcCurrAngle()           const { return CurrentArcAngle; }
	FORCEINLINE float GetTurnRate()               const { return CurrentTurnRate; }
	FORCEINLINE float GetJumpHorizontalVelocity() const { return WallJumpHorizontalVelocity; }
	FORCEINLINE float GetJumpVerticalVelocity()   const { return WallJumpVerticalVelocity; }
	/* Wall jump */
	bool StartWallJump();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ManageTick(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	UFUNCTION()
	void CheckForFloor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	void InitCharacterReference();
	/* Timed events */
	void StartDurationCountdown();
	void WallRunTimeout();
	void TemporarilyDisableWallRunning(const float DisableTime);
	void AllowWallRunning();
	void StartMonitoringSpeed();
	void MonitorSpeed();
	void StopMonitoringSpeed();
	void DelayedAirControl();
	void EnableWallJumpAirControl();
	/* Wall detection */
	void DetectWall(EWallSide Side);
	bool GetIsWallRunnable(const AActor* Wall) const;
	bool TopHeightCheck();
	bool BottomHeightCheck(EWallSide Side);
	bool LineTraceRelativeToCapsuleAndWall(const FVector& WallNormalCurrent, const FVector& WallLocationCurrent, const FWallRunTraceInfo& TraceInfo, const EWallSide& Side, FHitResult& HitResult, const bool bShowDebug) const;
	bool CheckCornerAngle(const FVector& FirstWallNormal, const FVector& SecondWallNormal);
	/* Wall run */
	bool StartWallRun(EWallSide Side);
	void StopWallRun(EMovementMode NewMovementMode, float TempDisableWallRunSec);
	void WallRunTick();
	void WallHug();
	/* Wall jump */
	void StopWallJump();
	void WallJumpTick(const float DeltaTime) const;
	/* Mics */
	void GatherPlayerInfo();
	void UpdateAnimationValues();

protected:
	/* Wall run parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallRunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float ArcAmount = 0.8f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallRunDurationLimit = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float OutsideCornerLimit = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float InsideCornerLimit = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallJumpAngle = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallJumpHorizontalVelocity = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallJumpVerticalVelocity = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	float WallJumpAirControl = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	bool bRequiresTag = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	FName TagForAllowingWallRun = "WallRun";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | WallRun")
	FName TagForPreventingWallRun = "NoWallRun";

private:
	UPROPERTY()
	AP7Character* Player;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	/* Original character information */
	float CapsuleRadius;
	float DefaultMaxFlySpeed;
	float DefaultMaxAcceleration;
	float DefaultBrakingDeceleration;
	float DefaultAirControl;
	bool  bDefaultOrientRotation;
	float PrimaryTraceSize;
	/* States */
	bool bWallRunning = false;
	bool bWallJumping = false;
	bool bWallRunningAvailable = true;
	bool bSpeedTooLow = false;
	/* Current wall information */
	EWallSide WallSide;
	FVector CurrentWallLocation;
	FVector PrimaryTraceNormal;
	FVector SecondaryTraceNormal;
	FVector AverageWallNormal;
	FVector PreviousVelocity;
	float CurrentCornerAngle;
	float LastKnownSafeHeight;
	float CurrentWallRunDuration;
	float DistanceFromWall;
	/* Wall run movement settings */
	float WallRunAcceleration = 50000.f;
	float WallRunBrakingDeceleration = 500.f;
	/* Timers */
	FTimerHandle DurationCountDownTimer;
	FTimerHandle DisableWallRunTimer;
	FTimerHandle SpeedMonitoringTimer;
	FTimerHandle WallJumpAirControlTimer;
	/* Animation values */
	float CurrentArcAngle;
	float CurrentTurnRate;
	FRotator PreviousRotation;
};
