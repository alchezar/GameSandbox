// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARAnimInstance.generated.h"

class UARAttributesComponent;

UCLASS()
class PROJECT06_API UARAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bAlive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Direction = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bJump;

private:
	UPROPERTY()
	APawn* Pawn;
	UPROPERTY()
	UARAttributesComponent* AttributesComponent;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
};
