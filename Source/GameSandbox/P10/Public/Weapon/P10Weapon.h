// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "P10Weapon.generated.h"

class UNiagaraSystem;

UENUM(BlueprintType)
enum class EP10FireMode : uint8
{
	Single  UMETA(DisplayName = "Single mode"),
	Brush   UMETA(DisplayName = "Brush mode"),
	Auto    UMETA(DisplayName = "Auto mode")
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FP10OnReloadHandle, APawn*, bool)

UCLASS()
class GAMESANDBOX_API AP10Weapon : public AActor
{
	GENERATED_BODY()

public:
	AP10Weapon();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE USkeletalMeshComponent* GetWeaponComponent() const { return WeaponMeshComponent; }
	FORCEINLINE FName GetFirstSocketName() const { return WeaponMeshComponent->GetAllSocketNames()[0]; }
	FORCEINLINE int32 GetNextFireCount() { return ++FireCount; }
	FORCEINLINE int32 GetCurrentAmmo() const { return Ammo; }
	FORCEINLINE int32 GetAmmoInClips() const { return AmmoInClips; }
	FORCEINLINE int32 GetClipCapacity() const { return ClipCapacity; }
	FORCEINLINE bool GetIsIsAmmoInfinite() const { return bInfinite; }
	virtual void StartFire();
	virtual void StopFire();
	virtual bool TryReload();
	void PlayMuzzleEffects() const;
	void DrawBeam(const FHitResult& Hit, const FVector& End);
	void PlayImpactEffect(const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	virtual void OneShot();

public:
	FP10OnReloadHandle OnReload;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USceneComponent* RootOffsetComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	FVector LocationOffset = FVector(-1.f, -10.f, -1.f);
	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	FRotator RotationOffset = FRotator(5.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* FireEffect;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	TSubclassOf<UCameraShakeBase> ShakeClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	TMap<TEnumAsByte<EPhysicalSurface>, UNiagaraSystem*> ImpactMap; 
	
	UPROPERTY(EditAnywhere, Category = "C++ | Beam")
	UNiagaraSystem* BeamEffect;
	UPROPERTY(EditAnywhere, Category = "C++ | Beam")
	FName TraceTargetName = "TraceTarget";

	UPROPERTY(EditAnywhere, Category = "C++ | Shoot")
	EP10FireMode FireMode = EP10FireMode::Single;
	UPROPERTY(EditAnywhere, Category = "C++ | Shoot", meta = (Units = "m/s"))
	float TimeBetweenShots = 0.1f;
	UPROPERTY(EditAnywhere, Category = "C++ | Shoot", meta = (Units = "deg"))
	float Spread = 3.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Shoot")
	float ShotRecoil = 0.2f;
	UPROPERTY(EditAnywhere, Category = "C++ | Shoot")
	float BaseDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Ammo")
	bool bInfinite = false;
	UPROPERTY(EditAnywhere, Category = "C++ | Ammo")
	int32 MaxAmmo = 120;
	UPROPERTY(EditAnywhere, Category = "C++ | Ammo", meta = (ClampMin = "1"))
	int32 ClipCapacity = 30;

private:
	FTimerHandle FireTimer;
	int32 FireCount = 0;
	int32 Ammo = 0;
	int32 AmmoInClips = 0;
};
