// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "TG_BaseCharacter.h"
#include "TG_FirstPersonCharacter.generated.h"

class ATG_Gun;
class UInputAction;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(config=Game)
class ATG_FirstPersonCharacter : public ATG_BaseCharacter
{
	GENERATED_BODY()

public:
	ATG_FirstPersonCharacter();
	UCameraComponent*       GetFirstPersonCameraComponent() const;

	// ATG_Gun* GetCurrentWeapon() const;
	bool GetHasRifle() const;
	void SetHasRifle(const bool bNewHasRifle);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const bool bRun);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	UCameraComponent* FP_CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FName HeadSocketName = "GoPro";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FVector CameraOffset = FVector(3.f, -25.f, 6.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* RunAction;

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	float MaxWalkSpeed = 0.f;
};
