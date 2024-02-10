// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnHealthChangedSignature, float /*NewHealth*/, float /*LastDamage*/, float /*HealthAlpha*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnHealthOverSignature, AController* /*Causer*/)

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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsAlive() const { return Health > 0.f; }

	FORCEINLINE bool GetIsHealthFull() const { return FMath::IsNearlyEqual(Health, MaxHealth); }
	FORCEINLINE float GetCurrentHealth() const { return Health; }
	FORCEINLINE float GetCurrentHealthAlpha() const { return Health / MaxHealth; }
	FORCEINLINE void SetMaxHealth(const float NewMaxHealth) { MaxHealth = NewMaxHealth; }

protected:
	virtual void ReceiveDamage(const float Damage, AController* Causer = nullptr);
	virtual void AddHealth(const float HealthAid);
	virtual void ChangeHealth(const float Power, AController* Causer);
	UFUNCTION()
	void OnOwnerTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void OnDead();

	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable)
	void Server_ChangeHealth(const float Power, AController* Causer);
	UFUNCTION(Server, Reliable)
	void Server_AddHealth(const float HealthAid);
	UFUNCTION(Server, Reliable)
	virtual void Server_ReceiveDamage(const float Damage, AController* Causer = nullptr);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeHealth(const float NewHealth, const float LastDamage, const float HealthAlpha);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HealthOver(AController* Causer);

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnHealthChangedSignature OnHealthChanged;
	FP13OnHealthOverSignature OnHealthOver;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Attribute", meta = (ClampMin = 1.f, UIMin = 1.f))
	float MaxHealth = 100.f;

private:
	UPROPERTY(Replicated)
	float Health = 0.f;
	UPROPERTY(Replicated)
	bool bAlive = true;
};
