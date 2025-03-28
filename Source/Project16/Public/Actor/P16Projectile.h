// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "P16Projectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PROJECT16_API AP16Projectile : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16Projectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement.Get(); }

protected:
	UFUNCTION()
	void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void PlayEffects() const;
	void ApplyDamageTo(AActor* Target) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Gameplay Effect", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USphereComponent> Sphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	TObjectPtr<UNiagaraSystem> ImpactEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	TObjectPtr<USoundBase> LoopingSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	TObjectPtr<USoundBase> ImpactSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Projectile")
	float Speed = 550.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Projectile")
	float LifeSpan = 15.f;

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComp = nullptr;

	bool         bHit = false;
	FTimerHandle DestroyTimer;
};
