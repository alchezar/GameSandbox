// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P10HealthComponent.generated.h"

class UP10HealthComponent;

DECLARE_MULTICAST_DELEGATE_SixParams(FP10OnHealthChangedSignature, UP10HealthComponent* /* Comp */, float /* Health */, float /* Delta */, const UDamageType* /* DamageType */, AController* /* InstigatedBy */, AActor* /* DamageCauser */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP10HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP10HealthComponent();
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

protected:
	UFUNCTION()
	void OnTakeDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	virtual void BeginPlay() override;

public:
	FP10OnHealthChangedSignature OnHealthChanged;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Health")
	float MaxHealth = 100.f;

private:
	float Health = 0.f;
};
