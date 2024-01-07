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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	UP13HealthComponent();

protected:
	virtual void BeginPlay() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FORCEINLINE float GetCurrentHealth() const { return Health; }
	FORCEINLINE void SetMaxHealth(const float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	virtual void ReceiveDamage(const float Damage);

protected:
	void OnDead();
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FP13OnHealthChangedSignature OnHealthChanged;
	FP13OnHealthOverSignature OnHealthOver;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (ClampMin = 1.f, UIMin = 1.f))
 	float MaxHealth = 100.f;

private:
	float Health = 0.f;
};
