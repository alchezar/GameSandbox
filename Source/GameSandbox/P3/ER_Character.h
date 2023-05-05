// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ER_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GAMESANDBOX_API AER_Character : public ACharacter
{
	GENERATED_BODY()

public:
	AER_Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;
	void MoveForward();
	void MoveLeft();
	void MoveRight();
	void MoveDown();

private:
	void SmoothlyChangeLane(int& LaneIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USpringArmComponent* CameraArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Input")
	UInputAction* ActionLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Input")
	UInputAction* ActionRight;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Input")
	UInputAction* ActionDown;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Input")
	UInputAction* ActionJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Move", meta = (Units = "s"))
	float JumpTime = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Move")
	float ChangeLineSpeed = 10.f;

private:
	FTimerHandle SlideTimer;
	int CurrentLaneIndex      = 1;
	float CurrentLanePosition = 0.f;
	TArray<float> LaneSwitchValues;
};
