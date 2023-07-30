// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "P8Kart.generated.h"

struct FInputActionValue;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP8Kart : public APawn
{
	GENERATED_BODY()

public:
	AP8Kart();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/* Multiplayer */
	void LocalMove(const FInputActionValue& Value);
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_Move(const FInputActionValue& Value);

private:
	void SetupComponents();
	void AddMappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MovementUpdate(const float DeltaTime);

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USceneComponent* SceneRoot;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* KartMesh;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* CameraView;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* BoxCollision;
#pragma endregion /* Component */
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
#pragma endregion/* Input */

	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "m/s"))
	float Speed = 20.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "kg"))
	float Mass = 1000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "N"))
	float MaxMoveForce = 10000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement", meta = (Units = "m"))
	float MinTurnRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement") // kg/m
	float DragCoefficient = 16.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement") // kg/m
	float RollingCoefficient = 0.015f;

private:
	FVector Velocity = FVector::ZeroVector;
	float MoveAlpha = 0.f; // Throttle
	float TurnAlpha = 0.f; // SteeringThrow
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
};
