// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "P17ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UBoxComponent;
struct FGameplayEventData;

UENUM()
enum class EP17ProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class PROJECT17_API AP17ProjectileBase : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17ProjectileBase();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	/// @par Callback ----------------------------------------------------------
	UFUNCTION()
	void OnProjectileBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnProjectileHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

private:
	void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++|Component")
	TObjectPtr<UBoxComponent> CollisionBox = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++|Component")
	TObjectPtr<UNiagaraComponent> NiagaraComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++|Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++|Projectile")
	EP17ProjectileDamagePolicy DamagePolicy = EP17ProjectileDamagePolicy::OnHit;
	UPROPERTY(BlueprintReadOnly, Category = "C++|Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle = {};
};
