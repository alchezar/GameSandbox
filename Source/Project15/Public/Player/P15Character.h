// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/P15Types.h"
#include "P15Character.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class PROJECT15_API AP15Character : public ACharacter
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP15Character();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* ------------------------------- This -------------------------------- */
protected:
	void MoveInput(const FInputActionValue& InputValue);
	void LookInput(const FInputActionValue& InputValue);
	void RunInput(const bool bRun);
	void CrouchInput();

private:
	void ChangeWalkSpeedSmoothly(const float DeltaTime);
	void UpdateCameraBoomOffsetSmoothly(const float DeltaTime);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> PlayerEye = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputMappingContext> InputContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> RunAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> CrouchAction = nullptr;

private:
	FP15SmoothChangeData SpeedChangeData        = {};
	FP15SmoothChangeData CameraOffsetChangeData = {};
	double               MaxCrouchOffset        = 0.0;
};
