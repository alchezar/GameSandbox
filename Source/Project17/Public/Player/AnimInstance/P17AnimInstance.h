// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P17AnimInstance.generated.h"

class UCharacterMovementComponent;

UCLASS()
class PROJECT17_API UP17AnimInstance : public UAnimInstance
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
	bool bCrouch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bMoving = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	float MinimalSpeed = 10.f;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> Owner = nullptr;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> Movement = nullptr;
};
