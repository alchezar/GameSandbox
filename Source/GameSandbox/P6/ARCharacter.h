// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAMESANDBOX_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);

private:
	void AddMappingContext();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Component")
	UCameraComponent* CameraComp;

#pragma region Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* JumpAction;
#pragma endregion // Input
};
