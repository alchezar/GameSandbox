// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P17CharacterBase.h"
#include "P17CharacterHero.generated.h"

class UMotionWarpingComponent;
class UP17UIHeroComponent;
class UP17CombatHeroComponent;
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

protected:
	/// @par AActor interface --------------------------------------------------
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/// @par APawn interface ---------------------------------------------------
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @par IP17CombatInterface -----------------------------------------------
	virtual UP17CombatPawnComponent* GetCombatComponent() const override;

	/// @par IP17UIInterface ---------------------------------------------------
	virtual UP17UIPawnComponent* GetPawnUIComponent() const override;
	virtual UP17UIHeroComponent* GetHeroUIComponent() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UP17CombatHeroComponent* GetHeroCombatComponent() const { return CombatComponent; };
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UP17UIHeroComponent* GetUIComponent() const { return UIComponent; };

protected:
	void ToggleUseControllerRotation(const bool bStart);
	void OnMoveCallback(const FInputActionValue& InputActionValue);
	void OnLookCallback(const FInputActionValue& InputActionValue);
	void OnAbilityInputPressedCallback(const FGameplayTag InInputTag);
	void OnAbilityInputReleasedCallback(const FGameplayTag InInputTag);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17CombatHeroComponent> CombatComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17UIHeroComponent> UIComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Data")
	TObjectPtr<UP17Data_InputConfig> InputConfig = nullptr;
};
