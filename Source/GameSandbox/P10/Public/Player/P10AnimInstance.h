// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P10AnimInstance.generated.h"

class AP10Character;

UCLASS()
class GAMESANDBOX_API UP10AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	float Speed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	float Direction = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	FRotator AimRotation = FRotator::ZeroRotator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bCrouch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bShooting = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bAiming = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	bool bDead = false;

private:
	UPROPERTY()
	APawn* Pawn;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
	UPROPERTY()
	AP10Character* PPCharacter;
};
