// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "P12WeaponBarrelComponent.generated.h"

class AP12Projectile;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FP12DecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Decal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Size = FVector(4.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Lifetime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeOutTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeScreenSize = 0.002f;
};

UENUM(BlueprintType)
enum class EP12HitRegistrationType : uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
struct FP12ShotInfo
{
	GENERATED_BODY()

	FP12ShotInfo()
		: Location10(FVector_NetQuantize100::ZeroVector), Direction(FVector_NetQuantizeNormal::ZeroVector) {}

	FP12ShotInfo(const FVector& Location, const FVector& Direction)
		: Location10(Location * 10.f), Direction(Direction) {}

	FORCEINLINE FVector GetLocation() const
	{
		return Location10 * 0.1f;
	}

	FORCEINLINE FVector GetDirection() const
	{
		return Direction;
	}

	UPROPERTY()
	FVector_NetQuantize100 Location10;
	UPROPERTY()
	FVector_NetQuantizeNormal Direction;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12WeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UP12WeaponBarrelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Shot(const FVector& ShotStart, const FVector& ShotDirection, const float SpreadAngle);

protected:
	virtual void BeginPlay() override;

private:
	void DrawNiagaraTale(const FVector& EndPoint);
	void HitScan(const FVector& Start, const FVector& End, const FVector& Direction, FHitResult& Out_HitResult);
	void ProjectileLaunch(const FVector& Start, const FVector& Direction);
	APawn* GetOwningPawn() const;
	AController* GetOwningController() const;
	void ProcessHit(const FHitResult& HitResult, const FVector& Direction);

	void ShotInternal(const TArray<FP12ShotInfo>& ShotsInfo);
	UFUNCTION(Server, Reliable)
	void Server_Shot(const TArray<FP12ShotInfo>& ShotsInfo);
	UFUNCTION()
	void OnRep_LastShotsInfo();

	UFUNCTION()
	void ProcessProjectileHit(const FHitResult& HitResult, const FVector& Direction, AP12Projectile* Projectile);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_DrawBulletHoleDecal(const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	float FiringRange = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	float DamageAmount = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel", meta = (ClampMin = 1, UIMin = 1))
	int32 BulletsPerShot = 1;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	EP12HitRegistrationType HitRegistrationType = EP12HitRegistrationType::HitScan;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel", meta = (EditCondition = "HitRegistrationType == EP12HitRegistrationType::Projectile"))
	TSubclassOf<AP12Projectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel", meta = (ClampMin = 1, UIMin = 1))
	int32 ProjectilePoolSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UNiagaraSystem* TraceNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Decal")
	FP12DecalInfo DecalInfo;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_LastShotsInfo")
	TArray<FP12ShotInfo> LastShotsInfo;

	FVector ProjectilePoolLocation = FVector(0.f, 0.f, -100.f);
	UPROPERTY(Replicated)
	TArray<AP12Projectile*> ProjectilePool;
	UPROPERTY(Replicated)
	int32 CurrentProjectileIndex = 0;
};
