// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P14Projectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class PROJECT14_API AP14Projectile : public AActor
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14Projectile();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	/// Save setter for the projectile's direction.
	/// @param Direction The projectile's direction.
	FORCEINLINE void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	/// Callback for projectile hit.
	/// @param HitComponent Our component that was hit.
	/// @param OtherActor Actor that was hit.
	/// @param OtherComp Component that was hit.
	/// @param NormalImpulse The normal impulse.
	/// @param Hit The hit result.
	UFUNCTION()
	void OnProjectileHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/* ------------------------------ Fields ------------------------------- */
protected:
	/// Sphere component for collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USphereComponent> CollisionComponent = nullptr;
	/// Mesh component for visual representation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
	/// Movement component for projectile.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UProjectileMovementComponent> MovementComponent = nullptr;
	/// How much damage the projectile does.
	UPROPERTY(EditAnywhere, Category = "C++")
	TObjectPtr<UNiagaraSystem> Particles = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DamageAmount = 30.f;
	/// How long the projectile exists.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (Unitis = s))
	float LifeSeconds = 5.f;

private:
	/// Direction the projectile is shot in.
	FVector ShotDirection = {};
};
