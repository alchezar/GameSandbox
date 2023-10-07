// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "P12SpiderPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UArrowComponent;
class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP12SpiderPawn : public APawn
{
	GENERATED_BODY()

public:
	AP12SpiderPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	FORCEINLINE float GetInputForward() const { return InputForward; }
	FORCEINLINE float GetInputRight() const { return InputRight; }
	FORCEINLINE float GetIKRightFrontFootOffset() const { return IKRightFrontFootOffset; }
	FORCEINLINE float GetIKLeftFrontFootOffset() const { return IKLeftFrontFootOffset; }
	FORCEINLINE float GetIKRightRearFootOffset() const { return IKRightRearFootOffset; }
	FORCEINLINE float GetIKLeftRearFootOffset() const { return IKLeftRearFootOffset; }

protected:
	virtual void BeginPlay() override;
	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput();

private:
	void SubsystemDefaultMappingContext() const;
	void RotatePawnToMovement(const float DeltaTime);
	float GetIKSocketOffset(const FName& SocketName);
	void DrawTraceDebug(const FHitResult& HitResult);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++")
	USphereComponent* SphereRootComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++")
	UPawnMovementComponent* MovementComponent;	
	UPROPERTY(VisibleAnywhere, Category = "C++")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++")
	UArrowComponent* ArrowComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category = "C++")
	float CollisionSphereRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	float RotationInterpolationSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputMappingContext* DefaultContext;	
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "C++")
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName RightFrontFootSocketName = "IK_FrontRightSocket";
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName RightRearFootSocketName = "IK_RearRightSocket";
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName LeftFrontFootSocketName = "IK_FrontLeftSocket";
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName LeftRearFootSocketName = "IK_RearLeftSocket";

private:
	float IKRightFrontFootOffset = 0.f;
	float IKRightRearFootOffset = 0.f;
	float IKLeftFrontFootOffset = 0.f;
	float IKLeftRearFootOffset = 0.f;

	float InputForward = 0.f;
	float InputRight = 0.f;
};
