// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARAnimInstance.generated.h"

class UARAttributesComponent;

UCLASS()
class GAMESANDBOX_API UARAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bAlive;

private:
	UPROPERTY()
	APawn* Pawn;
	UPROPERTY()
	UARAttributesComponent* AttributesComponent;
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;
};
