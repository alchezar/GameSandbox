// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class ATG_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATG_FirstPersonCharacter();
	UCameraComponent*       GetFirstPersonCameraComponent() const;

	ATG_Gun* GetCurrentWeapon() const;
	bool GetHasRifle() const;
	void SetHasRifle(const bool bNewHasRifle);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	UCameraComponent* FP_CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Mesh")
	TSubclassOf<ATG_Gun> WeaponClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Mesh")
	ATG_Gun* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FName HeadSocketName = "GoPro";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FName HandSocketName = "GripPoint";

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

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
};
