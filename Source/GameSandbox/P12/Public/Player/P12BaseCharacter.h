// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "P12BaseCharacter.generated.h"

class UP12BaseCharacterMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FP12CameraArmLength
{
	GENERATED_BODY()

	FP12CameraArmLength() : Crouch(150.f), Walk(200.f), Run(300.f) {}

	float Crouch = 0.f;
	float Walk = 0.f;
	float Run = 0.f;
};

UCLASS()
class GAMESANDBOX_API AP12BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AP12BaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	UP12BaseCharacterMovementComponent* GetBaseCharacterMovement() const;
	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput();
	void CrouchInput();
	void RunInput(const bool bRunRequest);

protected:
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;
	bool CanRun() const;

private:
	void ChangeCameraArmLength(const bool bStart, const float NewArmLength);
	void SmoothlyChangeCameraArmLength(const bool bRunStart, const float TargetLength);
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "C++ | Run")
	FP12CameraArmLength CameraArmLength;

private:
	TSoftObjectPtr<UP12BaseCharacterMovementComponent> BaseCharacterMovement;
	bool bCrouch = false;
	float DefaultMaxSpeed = 0.f;
	bool bRunning = false;
	FTimerHandle RunTimer;
};
