// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P12BaseCharacterAnimInstance.generated.h"

class UP12BaseCharacterMovementComponent;
class UCharacterMovementComponent;
class AP12BaseCharacter;

UCLASS()
class GAMESANDBOX_API UP12BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float Speed;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float Direction;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	bool bInAir;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	bool bCrouch;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	bool bLadder;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	float LadderSpeedRatio;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	FVector LeftLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	FVector RightLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "C++")
	FVector HipOffset = FVector::ZeroVector;
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	TWeakObjectPtr<UP12BaseCharacterMovementComponent> CachedCharacterMovement;
};
