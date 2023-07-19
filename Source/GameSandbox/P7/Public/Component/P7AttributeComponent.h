// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P7AttributeComponent.generated.h"

class AP7BaseCharacter;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP7OnReceiveDamageDelegate, ACharacter* /* Character */, float /* HealthPercent */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FP7OnStaminaUsedDelegate, ACharacter* /* Character */, float /* StaminaPercent */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FP7OnReceiveGoldDelegate, ACharacter* /* Character */, int32 /* Coins */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FP7OnReceiveSoulDelegate, ACharacter* /* Character */, int32 /* Souls */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP7AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP7AttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(const float Damage);
	void UseStamina(const float StaminaCost);
	void RefillStamina(const float StaminaRate);
	void ResetHealth();
	void ResetStamina();
	void AddCoins(const int32 NewCoins);
	void AddSoul(const int32 NewSouls);
	FORCEINLINE bool GetIsAlive() const { return CurrentHealth > 0.f; };
	FORCEINLINE float GetHealthPercent() const { return CurrentHealth / MaxHealth; };
	FORCEINLINE float GetStaminaPercent() const { return CurrentStamina / MaxStamina; };
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; };
	FORCEINLINE int32 GetCoins() const { return Coins; };
	FORCEINLINE int32 GetSouls() const { return Souls; };
	FORCEINLINE AP7BaseCharacter* GetOwnerCharacter() const { return Character; };
	void SetDefaultAttributes(ACharacter* OwnerCharacter);
	
protected:
	virtual void BeginPlay() override;
	void SetCharacter(ACharacter* OwnerCharacter);

public:
	FP7OnReceiveDamageDelegate OnReceiveDamage;
	FP7OnStaminaUsedDelegate   OnStaminaUsed;
	FP7OnReceiveGoldDelegate   OnReceiveGold;
	FP7OnReceiveSoulDelegate   OnReceiveSoul;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float CurrentHealth = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Stamina")
	float CurrentStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Stamina")
	float StaminaRegenRate = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Pickups")
	int32 Coins = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Pickups")
	int32 Souls = 0;

private:
	UPROPERTY()
	AP7BaseCharacter* Character;
};
