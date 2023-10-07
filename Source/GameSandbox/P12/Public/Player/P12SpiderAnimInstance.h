// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P12SpiderAnimInstance.generated.h"

class UP12SpiderMovementComponent;
class AP12SpiderPawn;

UCLASS()
class GAMESANDBOX_API UP12SpiderAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float InputForward;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float InputRight;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	bool bInAir;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++ | IK")
	FVector RightFrontFootEffectorLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++ | IK")
	FVector RightRearFootEffectorLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++ | IK")
	FVector LeftFrontFootEffectorLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++ | IK")
	FVector LeftRearFootEffectorLocation = FVector::ZeroVector;

private:
	TWeakObjectPtr<AP12SpiderPawn> CachedPawn;
	TWeakObjectPtr<UP12SpiderMovementComponent> CachedPawnMovement;
};
