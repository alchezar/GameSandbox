// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystemInterface.h"
#include "P12AttributeComponent.generated.h"

class AP12BaseCharacter;

DECLARE_MULTICAST_DELEGATE(FP12OnDeathSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12AttributeComponent : public UActorComponent, public IP12SaveSubsystemInterface
{
	GENERATED_BODY()

public:
	UP12AttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE bool GetIsAlive() const { return Health > 0.f; }
	float GetHealthPercent();
	void AddHealth(const float HealthToAdd);
	void RestoreFullStamina();

	virtual void OnLevelDeserialized_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void CheckIfDead();
	UFUNCTION()
	void OnRep_Health(float LastHealth);
	void OnHealthChanged();

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	void DebugDrawAttributes();
#endif

public:
	FP12OnDeathSignature OnDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Health", meta = (UIMin = 0.f))
	float MaxHealth = 100.f;

private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacterOwner;
	UPROPERTY(ReplicatedUsing = "OnRep_Health", SaveGame)
	float Health = 0.f;
};
