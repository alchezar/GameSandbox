// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P13/Public/Intearface/P13InputInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13TopDownCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

USTRUCT(BlueprintType)
struct FP13CameraHeightClamp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 1500.f;	
};

UCLASS()
class GAMESANDBOX_API AP13TopDownCharacter : public ACharacter, public IP13InputInterface
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13TopDownCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void MoveInput(const FVector2D MoveVector) override;
	virtual void SprintInput(const bool bStart) override;
	virtual void AimInput(const bool bStart) override;
	virtual void ZoomInput(const float Axis) override;
	
	virtual void RotateTowardMovement(const FVector& LookAtDirection) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE float GetIKLeftLegOffset() const { return IKLeftLegOffset; }
	FORCEINLINE float GetIKRightLegOffset() const { return IKRightLegOffset; }
	FORCEINLINE float GetIKHipOffset() const { return IKHitOffset; }

	void UpdateCharacterSpeed();
	void ChangeMovementState(const EP13MovementState NewMovementState);

private:
	void CreateComponents();
	void SetupCameraBoom() const;
	void ShakeCamera() const;

	/* Leg alignment. */
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f);
	void LegsIKFloorAlignment();

	void ZoomSmoothly(const float DeltaTime, const float FinalLength);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* TopDownCamera;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	bool bFlow = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float Height = 1000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	FP13CameraHeightClamp HeightClamp;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float ZoomStepDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	float SmoothCoefficient = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Camera")
	FRotator CameraRotation = FRotator(-80.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	TSubclassOf<UCameraShakeBase> ShakeClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	EP13MovementState MovementState = EP13MovementState::Run;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP13MovementSpeed MovementSpeed;

private:
	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
	float ZoomSteps = 0.f;
	FTimerHandle ScrollTimer;
	FTimerHandle ZoomTimer;
	EP13MovementState PreviousMovementState = MovementState;
};
