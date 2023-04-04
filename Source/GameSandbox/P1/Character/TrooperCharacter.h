// Copyright (C) 2023. IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TrooperCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GAMESANDBOX_API ATrooperCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Default

public:
	ATrooperCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma endregion // Default

#pragma region Component

public:
	USpringArmComponent* GetCameraSpring() const;
	UCameraComponent*    GetCamera() const;

protected:
	void SetupComponents();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* Spring;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

#pragma endregion // Component

#pragma region Input

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Jump() override;
	virtual void StopJumping() override;

	void MappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void DelayedJump();

	UFUNCTION(BlueprintCallable, Category = "Kinder")
	bool GetInJump() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	bool InJump = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Jump", meta = (AllowPrivateAccess = "true"))
	float JumpDelay = 1.f;

#pragma endregion // Input
};
