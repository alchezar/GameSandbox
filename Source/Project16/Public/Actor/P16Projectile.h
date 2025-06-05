// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/P16Type.h"
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
	virtual void Tick(const float DeltaSeconds) override;
	virtual void Destroyed() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement.Get(); }

	void UpdateHomingTarget(const AActor* HomingTarget, const FVector& MouseLocation);

protected:
	UFUNCTION()
	virtual void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void CheckTarget();
	virtual void PlayOnHitEffects() const;
	void         ApplyDamageTo(AActor* Target);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY()
	FP16DamageEffectParams DamageEffectParams = {};
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent = nullptr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	bool bHoming = true;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComp = nullptr;

private:
	FTimerHandle DestroyTimer;
};
