// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnHealthChangedSignature, float /*NewHealth*/, float /*LastDamage*/, float /*HealthAlpha*/)
DECLARE_MULTICAST_DELEGATE(FP13OnHealthOverSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13HealthComponent : public UActorComponent
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13HealthComponent();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE bool GetIsHealthFull() const { return FMath::IsNearlyEqual(Health, MaxHealth); }
	FORCEINLINE float GetCurrentHealth() const { return Health; }
	FORCEINLINE float GetCurrentHealthAlpha() const { return Health / MaxHealth; }
	FORCEINLINE void SetMaxHealth(const float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	virtual void ReceiveDamage(const float Damage);
	virtual void AddHealth(const float HealthAid);
	virtual void ChangeHealth(const float Power);

protected:
	void OnDead();

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnHealthChangedSignature OnHealthChanged;
	FP13OnHealthOverSignature OnHealthOver;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Attribute", meta = (ClampMin = 1.f, UIMin = 1.f))
	float MaxHealth = 100.f;

private:
	float Health = 0.f;
};
