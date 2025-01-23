// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P9PunchAnimInstance.generated.h"

class UCharacterMovementComponent;
class AP9PunchCharacter;

UCLASS()
class PROJECT09_API UP9PunchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float Direction = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bInAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bArmed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bCrouching = false;
		
private:
	UPROPERTY()
	AP9PunchCharacter* Owner;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
};
