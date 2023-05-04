// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ER_AnimInstance.generated.h"

class APawn;
class UPawnMovementComponent;

UCLASS()
class GAMESANDBOX_API UER_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Kinder | Movement")
	float Speed = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Kinder | Movement")
	bool bInAir = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Kinder | Movement")
	APawn* Pawn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Kinder | Movement")
	UPawnMovementComponent* MovementComponent;
};
