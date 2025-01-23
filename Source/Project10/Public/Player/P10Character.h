// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Util/P10Library.h"
#include "P10Character.generated.h"

class UP10HealthComponent;
class AP10Weapon;
class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;
class AP10Projectile;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class  EP10CharMask : uint8
{
	Idle   = 0b00000000 UMETA(DisplayName = "Idle"),       // 0
	Jump   = 0b00000001 UMETA(DisplayName = "Jumpint"),	   // 1
	Crouch = 0b00000010 UMETA(DisplayName = "Crouching"),  // 2
	Run    = 0b00000100 UMETA(DisplayName = "Running"),    // 4
	Aim    = 0b00001000 UMETA(DisplayName = "Aiming"), 	   // 8
	Shoot  = 0b00010000 UMETA(DisplayName = "Shooting"),   // 16
	Dead   = 0b00100000 UMETA(DisplayName = "Dead")        // 32
};
ENUM_CLASS_FLAGS(EP10CharMask)

UCLASS()
class PROJECT10_API AP10Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP10Character();
	void OnHealthChangedHandle(UP10HealthComponent* Component, float Health, float Delta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual FVector GetPawnViewLocation() const override;
	FORCEINLINE bool GetIsDead() const { return UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Dead); }
	FORCEINLINE bool GetIsShooting() const { return UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Shoot); }
	FORCEINLINE bool GetIsAiming() const { return UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Aim); }
	FORCEINLINE bool GetIsCarryingObjective() const { return bCarryingObjective; }
	FORCEINLINE void SetCarryingObjective(const bool bNewCarrying) { bCarryingObjective = bNewCarrying; }
	UFUNCTION(BlueprintCallable, Category = "C++")
	void FireInput(bool bShoot);
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();

private:
	void LookInput(const FInputActionValue& Value);
	void MoveInput(const FInputActionValue& Value);
	void JumpInput(bool bStart);
	void CrouchInput();
	void AimInput(bool bAim);
	void ReloadInput();
	void SpawnWeapon();
	void ZoomSmoothlyHandle();
	void OnWeaponReloadHandle(APawn* WeaponOwner, bool bAccess);

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USpringArmComponent* ArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UPawnNoiseEmitterComponent* NoiseEmitter;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP10HealthComponent* HealthComponent;

protected:
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
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* ReloadAction;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "C++ | Interact")
	bool bCarryingObjective;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	TSubclassOf<AP10Weapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName HandSocketName = "GripPoint";

private:
	UPROPERTY(Replicated)
	AP10Weapon* Weapon;
	UPROPERTY(Replicated)
	EP10CharMask CharStateMask = EP10CharMask::Idle;
	float TargetFOV = 0.f;
	FTimerHandle ZoomTimer;
};
