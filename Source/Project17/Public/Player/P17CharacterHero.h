// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17CharacterBase.h"
#include "P17CharacterHero.generated.h"

struct FInputActionValue;
class UP17Data_InputConfig;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECT17_API AP17CharacterHero : public AP17CharacterBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17CharacterHero();

	/// @par AActor interface
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/// @par APawn interface
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	void ToggleUseControllerRotation(const bool bStart);
	void OnMoveCallback(const FInputActionValue& InputActionValue);
	void OnLookCallback(const FInputActionValue& InputActionValue);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Data")
	TObjectPtr<UP17Data_InputConfig> InputConfig = nullptr;
};
