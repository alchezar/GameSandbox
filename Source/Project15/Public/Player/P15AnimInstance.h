// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P15AnimInstance.generated.h"

class AP15Character;
class UCharacterMovementComponent;

UCLASS()
class PROJECT15_API UP15AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(const float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	float Speed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	float Direction = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bMoving = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bCrouch = false;

private:
	TObjectPtr<AP15Character>               Owner;
	TObjectPtr<UCharacterMovementComponent> Movement;
};
