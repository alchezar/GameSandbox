// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "P10Character.generated.h"

class AP10Weapon;
class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;
class AP10Projectile;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class GAMESANDBOX_API AP10Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP10Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE bool GetIsShooting() const { return bShooting; }
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	FORCEINLINE bool GetIsCarryingObjective() const { return bCarryingObjective; }
	FORCEINLINE void SetCarryingObjective(const bool bNewCarrying) { bCarryingObjective = bNewCarrying; }

protected:
	virtual void BeginPlay() override;
	void FireInput(bool bShoot);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();

private:
	void LookInput(const FInputActionValue& Value);
	void MoveInput(const FInputActionValue& Value);
	void CrouchInput();
	void AimInput(bool bAim);
	void SpawnWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* ArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPawnNoiseEmitterComponent* NoiseEmitter;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	TSubclassOf<AP10Weapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	TSubclassOf<AP10Projectile> ProjectileClass;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "C++ | Weapon")
	bool bCarryingObjective;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName MuzzleSocketName = "MuzzleSocket";
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Weapon")
	UNiagaraSystem* FireEffect;

private:
	bool bShooting = false;
	bool bAiming = false;
	bool bCrouch = false;
	UPROPERTY()
	AP10Weapon* Weapon;
};
