// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "STU_HealthComponent.generated.h"

class UPhysicalMaterial;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	USTU_HealthComponent();
	UFUNCTION(BlueprintCallable, Category = "Kinder | Health")
	bool IsDead() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Health")
	float GetHealthPercent() const;
	float GetHealth() const;
	bool TryToAddHealth(float PickedHealth);
	bool IsHealthFull() const;
	void Killed(const AController* KillerController) const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser);


	void SetHealth(float NewHealth);
	void Healing();
	void PlayCameraShake() const;
	void ApplyDamage(const float Damage, const AController* InstigatedBy);
	float GetPointDamageModifier(AActor* Victim, const FName& BoneName);
	void ReportDamageEvent(const float Damage, const AController* InstigatedBy);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Health", meta = (ClampMin = 0.0, ClampMax = 200.0))
	float MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Health")
	TMap<UPhysicalMaterial*, float> DamageModifiers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal")
	bool AutoHeal = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal", Units = "Seconds"))
	float HealUpdateDelay = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal", Units = "Seconds"))
	float HealStartDelay = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Heal", meta = (EditCondition = "AutoHeal"))
	float HealAmount = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

private:
	float Health = 0.f;
	FTimerHandle HealTimer;
};
