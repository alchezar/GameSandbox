// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Utils/P14Types.h"
#include "P14Character.generated.h"

class UInputMappingContext;
class UInputAction;
class UP14InventoryComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECT14_API AP14Character : public ACharacter
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14Character();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	_NODISCARD TObjectPtr<UCameraComponent>    GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "C++")
	float GetHealthPercent() const { return Health / HealthData.MaxHealth; }

protected:
	void MoveInput(const FInputActionValue& InputValue);
	void LookInput(const FInputActionValue& InputValue);

	UFUNCTION()
	void OnAnyDamageReceivedCallback(AActor* DamagedActor, const float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	void UpdateHealth(const float HealthDelta);
	void OnHealing();
	void OnDeath();

	/* ------------------------------ Fields ------------------------------- */
public:
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, Category = "C++")
	FP14OnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UP14InventoryComponent> InventoryComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	float BaseTurnRate = 45.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	float BaseLookUpRate = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputMappingContext> InputContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputAction> LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UInputAction> JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FP14HealthData HealthData = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	EP14ClothType ClothType = EP14ClothType::Skeletal;

private:
	float        Health      = 0.f;
	FTimerHandle HealthTimer = {};
};
