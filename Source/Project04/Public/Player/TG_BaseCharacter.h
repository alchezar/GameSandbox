// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TG_BaseCharacter.generated.h"

class ATG_Gun;

UCLASS()
class PROJECT04_API ATG_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATG_BaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ATG_Gun* GetCurrentWeapon() const;
	bool GetIsAiming() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | AI")
	bool GetIsDead () const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | AI")
	void SetIsAiming(const bool bNewAim);
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	virtual void ReceiveDamage(const float Damage);
	virtual void SetHealth(const float NewHealth);
	float GetHealth() const;
	float GetMaxHealth() const;

	

protected:
	virtual void BeginPlay() override;
	virtual void CharacterDying();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Mesh")
	TSubclassOf<ATG_Gun> WeaponClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Mesh")
	ATG_Gun* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Attach")
	bool bAim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	bool bDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Health")
	float Health = 0.f;

private:
};
