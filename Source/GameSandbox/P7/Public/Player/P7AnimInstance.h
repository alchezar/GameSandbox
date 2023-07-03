// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "P7AnimInstance.generated.h"

class UCharacterMovementComponent;
class AP7Character;

UCLASS()
class GAMESANDBOX_API UP7AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	float GetMovementDirection();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float Direction = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bJump = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bDoubleJump = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | State")
	TEnumAsByte<ECharacterState> CharacterState = ECharacterState::ECS_Unequipped;

private:
	UPROPERTY()
	ACharacter* Character;
	UPROPERTY()
	AP7Character* Player;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
};
