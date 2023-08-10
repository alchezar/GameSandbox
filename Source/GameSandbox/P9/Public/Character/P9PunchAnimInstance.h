// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P9PunchAnimInstance.generated.h"

class UCharacterMovementComponent;
class AP9PunchCharacter;

UCLASS()
class GAMESANDBOX_API UP9PunchAnimInstance : public UAnimInstance
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

private:
	UPROPERTY()
	APawn* Owner;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
};
