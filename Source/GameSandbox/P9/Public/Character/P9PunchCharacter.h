// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P9/Public/Util/P9Utils.h"
#include "P9PunchCharacter.generated.h"

class USphereComponent;
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
	float GetMovementDirectionAngle();

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();
	void OnPunchHandle(USkeletalMeshComponent* MeshComp, bool bStart);

private:
	void SetupComponents();
	void CheckHardReferences();
	void AddMappingContext();
	void InitAnimNotifies();
	
	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param Level affects color of log
	 * @param Message the message for display
	 */
	void Log(EP9LogLevel Level, const FString& Message);
	/**
	 * Log - prints a message to all the log outputs with a specific color
	 * @param Level affects color of log
	 * @param Message the message for display
	 * @param LogOutput - All, Output Log or Screen
	 */
	void Log(const EP9LogLevel Level,  const FString& Message, const EP9LogOutput LogOutput);

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* FollowCamera;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* LeftFistCollision;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* RightFistCollision;	
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
	EP9CharState CharState = EP9CharState::IDLE;
};
