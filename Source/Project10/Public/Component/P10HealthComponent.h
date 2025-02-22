// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P10HealthComponent.generated.h"

class UP10HealthComponent;

DECLARE_MULTICAST_DELEGATE_SixParams(FP10OnHealthChangedSignature, UP10HealthComponent* /*Comp*/, float /*Health*/, float /*Delta*/, const UDamageType* /*DamageType*/, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT10_API UP10HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP10HealthComponent();
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(Blueprintable, BlueprintPure, Category = "C++")
	static bool IsFriendly(const AActor* ActorA, const AActor* ActorB);

protected:
	UFUNCTION()
	void OnTakeDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnHealthChanged(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnRep_Health(const float OldHealth);
	
public:
	FP10OnHealthChangedSignature OnHealthChanged;
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "C++ | Team")
	uint8 TeamNum;
	
protected:
	UPROPERTY(ReplicatedUsing = "OnRep_Health", EditAnywhere, Category = "C++ | Health")
	float MaxHealth = 100.f;

private:
	UPROPERTY(Replicated)
	float Health = 0.f;
};
