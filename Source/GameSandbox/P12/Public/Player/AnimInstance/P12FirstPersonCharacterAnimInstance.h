// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12BaseCharacterAnimInstance.h"
#include "P12FirstPersonCharacterAnimInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP12FirstPersonCharacterAnimInstance : public UP12BaseCharacterAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float CalculateCameraPitchAngle() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++ | FirstPerson")
	float PlayerCameraPitchAngle = 0.f;
};
