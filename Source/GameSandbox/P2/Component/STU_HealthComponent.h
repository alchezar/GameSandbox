// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STU_HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_HealthComponent();

	FOnDeathSignature         OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Kinder | Health")
	bool IsDead() const;

	float GetHealth() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Health", meta = (ClampMin = 0.0, ClampMax = 200.0))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal")
	bool AutoHeal = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal", Units = "Seconds"))
	float HealUpdateDelay = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal", Units = "Seconds"))
	float HealStartDelay = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal"))
	float HealAmount = 1.f;

private:
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void SetHealth(float NewHealth);
	void Healing();

	float        Health = 0.f;
	FTimerHandle HealTimer;
};
