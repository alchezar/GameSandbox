// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/P13Types.h"
#include "P13CharAnimInstance.generated.h"

class AP13CharacterBase;
class AP13TopDownCharacter;

UCLASS()
class PROJECT13_API UP13CharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/* ------------------------------- This -------------------------------- */
private:
	float GetMovementDirectionAngle() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	bool bInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	bool bCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	bool bAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	EP13MovementState MovementState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	FRotator AimRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	EP13AmmoType WeaponType;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector LeftLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector RightLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector HipOffset = FVector::ZeroVector;

private:
	TWeakObjectPtr<AP13CharacterBase> CachedCharacter;
};
