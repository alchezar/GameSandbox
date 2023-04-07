// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STU_BaseCharacter.generated.h"

class UTextRenderComponent;
class USTU_HealthComponent;
class USTU_WeaponComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GAMESANDBOX_API ASTU_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTU_BaseCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma region Component

public:
	UCameraComponent* GetCameraComp() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USTU_HealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	UTextRenderComponent* HealthTextComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	USTU_WeaponComponent* WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Components")
	TArray<UAnimMontage*> DeathAnimations;

private:
	void SetupComponent();

#pragma endregion // Component

#pragma region Input

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Kinder | Input")
	bool GetIsRunning() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Movement")
	float GetMovementDirection() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Input")
	UInputAction* FireAction;

private:
	void MappingContext() const;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchToggle();
	void StartRun();
	void StopRun();

	bool bRunning = false;

#pragma endregion // Input

#pragma region Health

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	float LifeSpanOnDeath = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Damage")
	FVector2D LandedDamage = FVector2D(10.f, 100.f);

private:
	UFUNCTION()
	void LandedHandle(const FHitResult& Hit);

	void OnDeathHandle();
	void OnHealthChangedHandle(float Health);

#pragma endregion // Health

};
