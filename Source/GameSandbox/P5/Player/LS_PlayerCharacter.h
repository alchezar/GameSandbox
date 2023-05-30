// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "LS_BaseCharacter.h"
#include "LS_PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAMESANDBOX_API ALS_PlayerCharacter : public ALS_BaseCharacter
{
	GENERATED_BODY()

public:
	ALS_PlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetTurnLeft() const;
	bool GetTurnRight() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Run(bool bRun);

private:
	void TurnInPlace();
	void EnableTurning(bool bUseDesire, bool bOrient, float Rate = 360.f);

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* RunAction;

private:
	float MaxWalkSpeed = 0.f;
	bool bTurnLeft = false;
	bool bTurnRight = false;
};
