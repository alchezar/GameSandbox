// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "P10/Public/Util/P10Library.h"
#include "P10Character.generated.h"

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
	Shoot  = 0b00010000 UMETA(DisplayName = "Shooting")    // 16
};
ENUM_CLASS_FLAGS(EP10CharMask)

UCLASS()
class GAMESANDBOX_API AP10Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP10Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual FVector GetPawnViewLocation() const override;
	FORCEINLINE bool GetIsShooting() const { return UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Shoot); }
	FORCEINLINE bool GetIsAiming() const { return UP10Library::BitflagIsActive(CharStateMask, EP10CharMask::Aim); }
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
	void JumpInput(bool bStart);
	void CrouchInput();
	void AimInput(bool bAim);
	void SpawnWeapon();
	void ZoomSmoothlyHandle();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USpringArmComponent* ArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "C++ | Interact")
	bool bCarryingObjective;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	TSubclassOf<AP10Weapon> WeaponClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName HandSocketName = "GripPoint";

private:
	UPROPERTY()
	AP10Weapon* Weapon;
	EP10CharMask CharStateMask = EP10CharMask::Idle;
	float TargetFOV = 0.f;
	FTimerHandle ZoomTimer;
};
