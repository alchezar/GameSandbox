// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LS_AnimInstance.generated.h"

class APawn;
class UPawnMovementComponent;

UCLASS()
class GAMESANDBOX_API ULS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirectionAngle();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	float Direction = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	bool bJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	bool bDoubleJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	bool bDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	bool bTurnLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kidner")
	bool bTurnRight = false;

private:
	UPROPERTY()
	APawn* Pawn;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
};
