// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Player/AnimInstance/P17AnimInstanceCharacter.h"
#include "P17AnimInstanceHero.generated.h"

class AP17CharacterHero;

UCLASS()
class PROJECT17_API UP17AnimInstanceHero : public UP17AnimInstanceCharacter
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(const float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bCrouch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bMoving = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	float FallingSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Threshold")
	float MoveThreshold = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Threshold")
	float FallThreshold = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Relax")
	bool bCanRelax = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Relax")
	float IdleElapsed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Relax")
	float RelaxThreshold = 5.f;

private:
	UPROPERTY()
	TObjectPtr<AP17CharacterHero> OwnerHero = nullptr;
};
