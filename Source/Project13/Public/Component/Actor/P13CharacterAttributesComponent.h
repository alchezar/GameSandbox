// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13HealthComponent.h"
#include "P13CharacterAttributesComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnShieldChangesHandle, float /*NewShield*/, float /*LastDamage*/, float /*ShieldAlpha*/)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT13_API UP13CharacterAttributesComponent : public UP13HealthComponent
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13CharacterAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE bool GetShieldIsActive() const { return Shield > 0.f; }
	FORCEINLINE float GetCurrentShield() const { return Shield; }
	FORCEINLINE float GetCurrentShieldAlpha() const { return Shield / MaxShield; }

protected:
	virtual void ReceiveDamage(const float Damage, AController* Causer = nullptr) override;

private:
	void ShieldRecoveryTick(const float DeltaTime);

	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnShieldChangedBroadcast(const float NewShield, const float LastDamage, const float ShieldAlpha);

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnShieldChangesHandle OnShieldChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Attribute", meta = (ClampMin = 1.f, UIMin = 1.f))
	float MaxShield = 50.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Attribute")
	float RecoveryStartDelay = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Attribute")
	float RecoveryTime = 5.f;

private:
	UPROPERTY(Replicated)
	float Shield = 0.f;
	FTimerHandle StartRecoveryTimer;
	bool bRecovering = false;
};
