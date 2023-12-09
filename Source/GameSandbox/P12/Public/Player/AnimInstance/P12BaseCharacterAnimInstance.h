// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12BaseCharacterAnimInstance.generated.h"

class UP12EquipmentComponent;
class UP12BaseCharacterMovementComponent;
class UCharacterMovementComponent;
class AP12BaseCharacter;

UCLASS()
class GAMESANDBOX_API UP12BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void OnLoadoutCreatedHandle(UP12EquipmentComponent* Equipment);

protected:
	FORCEINLINE TWeakObjectPtr<AP12BaseCharacter> GetCachedCharacter() const { return CachedCharacter; }

private:
	float GetMovementDirectionAngle() const;

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
	bool bLadder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	bool bAim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	float LadderSpeedRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	EP12EquipableItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	FRotator AimRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "C++")
	FTransform GripSocketTransform;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector LeftLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector RightLegOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "C++")
	FVector HipOffset = FVector::ZeroVector;

private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	TWeakObjectPtr<UP12BaseCharacterMovementComponent> CachedCharacterMovement;
	TWeakObjectPtr<UP12EquipmentComponent> CachedEquipmentComponent;
};
