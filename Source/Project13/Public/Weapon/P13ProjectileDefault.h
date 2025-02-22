// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/P13Types.h"
#include "P13ProjectileDefault.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UENUM(Blueprintable)
enum class EP13ProjectileDamageType : uint8
{
	Point,
	Radial,
	Grenade
};

UCLASS()
class PROJECT13_API AP13ProjectileDefault : public AActor
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13ProjectileDefault();
	virtual void PostInitializeComponents() override;
	// virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE UProjectileMovementComponent* GetBulletMovement() const { return BulletMovement; };
	FORCEINLINE TSubclassOf<UP13StateEffect> GetBulletStateEffect() const { return BulletSettings.StateEffectClass; }
	void InitBullet(const float NewLifeSpan, const FP13ProjectileInfo& NewBulletSettings);

protected:
	UFUNCTION()
	void OnBulletHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnBulletBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBulletEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

private:
	void SpawnEffectsOnHit(const FHitResult& Hit);
	void ImpactProjectile();

	void GivePointDamage(const FHitResult& Hit, AActor* OtherActor);
	void GiveRadialDamage();

	void SpawnStateEffect(const FHitResult& Hit, AController* Causer);

	/* ------------------------------ Network ------------------------------ */
	UFUNCTION(Server, Reliable)
	void Server_SpawnEffectsOnHit(const FHitResult& Hit);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnEffectsOnHit(UMaterialInterface* Decal, UNiagaraSystem* Particle, USoundBase* Sound, const FVector& Location, const FRotator& Rotation, UPrimitiveComponent* AnchorComp);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USphereComponent* BulletCollision;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* BulletMesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UParticleSystemComponent* BulletTrace = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UProjectileMovementComponent* BulletMovement;

	UPROPERTY(EditAnywhere, Category = "C++ | Fire")
	FP13ProjectileInfo BulletSettings;

	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	EP13ProjectileDamageType DamageType = EP13ProjectileDamageType::Point;

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UParticleSystem* ExplodeEffect = nullptr;

private:
	FTimerHandle ExplodeTimer;
	FHitResult ProjectileHit;
};
