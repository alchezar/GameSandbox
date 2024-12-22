// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P7AnimInstanceWallRun.generated.h"

class AP7Character;
class UP7WallRunComponent;
enum EWallSide : uint8;

UCLASS()
class GAMESANDBOX_API UP7AnimInstanceWallRun : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	bool bWallRun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	bool bWallJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	TEnumAsByte<EWallSide> WallSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float CurrentSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float CurrentAnimPlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float CurrentArcAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float WallJumpAnimSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | WallRun")
	float HandCorrectionAdditive;

private:
	UPROPERTY()
	AP7Character* Player;
	UPROPERTY()
	UP7WallRunComponent* WallRunComponent;
};
