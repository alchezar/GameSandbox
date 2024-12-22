// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P7Bird.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
class UFloatingPawnMovement;

UCLASS()
class GAMESANDBOX_API AP7Bird : public APawn
{
	GENERATED_BODY()

public:
	AP7Bird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupComponents();
	void AddMappingContext() const;
	void MovePawn(const FInputActionValue& Value);
	void LookPawn(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCapsuleComponent* Capsule;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* BirdMesh;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* CameraBoom;	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* ViewCamera;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UFloatingPawnMovement* FloatingPawnMovement;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
};
