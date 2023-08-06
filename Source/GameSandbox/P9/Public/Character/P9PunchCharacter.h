// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P9/Public/Util/P9Utils.h"
#include "P9PunchCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class GAMESANDBOX_API AP9PunchCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AP9PunchCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(bool bAttack);

private:
	void SetupComponents();
	void CheckHardReferences();
	void AddMappingContext();

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* FollowCamera;
#pragma endregion /* Component */
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* AttackAction;
#pragma endregion /* Input */
#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Montage")
	UAnimMontage* MeleeFistAttackMontage;
#pragma endregion /* Montage */

private:
	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param LogLevel affects color of log
	 * @param Message the message for display
	 */
	void Log(EP9LogLevel LogLevel, const FString& Message);
	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param LogLevel affects color of log
	 * @param Message the message for display
	 * @param LogOutput - All, Output Log or Screen
	 */
	void Log(EP9LogLevel LogLevel, const FString& Message, EP9LogOutput LogOutput);
};
