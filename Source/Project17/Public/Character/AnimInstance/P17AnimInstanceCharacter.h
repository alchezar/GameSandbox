// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17AnimInstanceBase.h"
#include "P17AnimInstanceCharacter.generated.h"

class UCharacterMovementComponent;
class AP17CharacterBase;

UCLASS()
class PROJECT17_API UP17AnimInstanceCharacter : public UP17AnimInstanceBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(const float DeltaSeconds) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	float Speed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Movement")
	bool bAcceleration = false;

	UPROPERTY()
	TObjectPtr<ACharacter> Owner = nullptr;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> Movement = nullptr;
};
