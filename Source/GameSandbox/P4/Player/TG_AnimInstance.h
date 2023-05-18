// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TG_AnimInstance.generated.h"

class APawn;
class UPawnMovementComponent;

UCLASS()
class GAMESANDBOX_API UTG_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	float GetMovementDirectionAngle() const;
	void OnFiringHandle(const bool bFire);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	float Speed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	float Direction = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	FRotator AimRotation = FRotator::ZeroRotator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	bool bInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	bool bShooting = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	bool bAiming = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	bool bDead = false;

private:
	UPROPERTY()
	APawn* Pawn;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
};
