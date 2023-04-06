// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STU_BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GAMESANDBOX_API ASTU_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTU_BaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma region Component

public:
	UCameraComponent* GetCameraComp() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	UCameraComponent* CameraComponent;

private:
	void SetupComponent();

#pragma endregion // Component

#pragma region Input

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Kinder | Input")
	bool GetIsRunning() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Movement")
	float GetMovementDirection()const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* CrouchAction;

private:
	void MappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchToggle();
	void StartRun();
	void StopRun();

	bool bRunning = false;

#pragma endregion // Input
};
