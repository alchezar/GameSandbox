// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "P8Kart.generated.h"

class UP8ReplicatorComponent;
class UP8MovementComponent;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class GAMESANDBOX_API AP8Kart : public APawn
{
	GENERATED_BODY()

public:
	AP8Kart();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupComponents();
	void AddMappingContext() const;
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

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
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP8MovementComponent* MoveComp;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UP8ReplicatorComponent* RepComp;
#pragma endregion /* Component */
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
#pragma endregion/* Input */
	
private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
};
