// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "TG_BaseCharacter.h"
#include "TG_FirstPersonCharacter.generated.h"

class UPostProcessComponent;
class UBoxComponent;
class ATG_GameMode;
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

DECLARE_MULTICAST_DELEGATE_OneParam(FTGOnHealthChangedSignature, float);

UCLASS(config=Game)
class ATG_FirstPersonCharacter : public ATG_BaseCharacter
{
	GENERATED_BODY()

public:
	ATG_FirstPersonCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	UCameraComponent* GetFirstPersonCameraComponent() const;

	// ATG_Gun* GetCurrentWeapon() const;
	bool GetHasRifle() const;
	void SetHasRifle(const bool bNewHasRifle);
	virtual void ReceiveDamage(const float Damage) override;
	virtual void SetHealth(const float NewHealth) override;

	float GetCurrentHealth() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void CharacterDying() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const bool bRun);

private:
	void ApplyDamageEffect();
	void PlayDamageEffect();

public:
	FTGOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	UCameraComponent* FP_CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Camera")
	UArrowComponent* DeathCameraPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FName HeadSocketName = "GoPro";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FVector CameraOffset = FVector(3.f, -25.f, 6.f);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Input")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder |Post")
	UBoxComponent* EffectVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder |Post")
	UPostProcessComponent* DamagePostProcess;

private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	float MaxWalkSpeed = 0.f;
	float Alpha = 0.f;
	FTimerHandle DamageTimer;
};
