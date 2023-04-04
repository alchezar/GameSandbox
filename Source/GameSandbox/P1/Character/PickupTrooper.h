// Copyright (C) 2023. IKinder

#pragma once

#include "CoreMinimal.h"
#include "TrooperCharacter.h"
#include "GameFramework/Character.h"
#include "PickupTrooper.generated.h"

UCLASS()
class GAMESANDBOX_API APickupTrooper : public ATrooperCharacter
{
	GENERATED_BODY()

#pragma region Default

public:
	APickupTrooper();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma endregion // Default

#pragma region Input

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CollectAction;

#pragma endregion // Input

#pragma region Pickup

public:
	UFUNCTION(BlueprintCallable)
	USphereComponent* GetSphereCollision() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Kinder | Pickup")
	void BindCollectPickups();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Pickup", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;

#pragma endregion // Pickup

#pragma region Power

public:
	UFUNCTION(BlueprintPure, Category = "Kinder | Power")
	float GetBasePowerLevel() const;
	UFUNCTION(BlueprintPure, Category = "Kinder | Power")
	float GetCurrentPowerLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Power")
	void UpdateCurrentPowerLevel(const float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Power")
	float BasePowerLevel = 2000.f;
	UPROPERTY(VisibleAnywhere, Category = "Kinder | Power")
	float CurrentPowerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Power")
	float BaseSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Power")
	float SpeedMultiplier = 0.3f;

#pragma endregion // Power

#pragma region Effect

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FLinearColor StartColor = FLinearColor(0.f, 1.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FLinearColor EndColor = FLinearColor(1.f, 0.f, 0.f);
	UPROPERTY(BlueprintReadWrite, Category = "Kinder | Effect")
	UMaterialInstanceDynamic* DynamicMaterial1;
	UPROPERTY(BlueprintReadWrite, Category = "Kinder | Effect")
	UMaterialInstanceDynamic* DynamicMaterial2;
	UPROPERTY(BlueprintReadWrite, Category = "Kinder | Effect")
	UMaterialInstanceDynamic* DynamicMaterial3;

private:
	void CreateDynamicMaterials();
	void UpdateDynamicMaterial();

#pragma endregion // Effects
};
