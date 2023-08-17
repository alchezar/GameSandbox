// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "P10Character.generated.h"

class UInputAction;
class UInputMappingContext;
class AP10Projectile;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAMESANDBOX_API AP10Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP10Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	void Fire();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USpringArmComponent* ArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* LookAction;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	TSubclassOf<AP10Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	USoundBase* FireSound;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "C++ | Weapon")
	bool bCarryingObjective;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon")
	FName MuzzleSocketName = "MuzzleSocket";
};
