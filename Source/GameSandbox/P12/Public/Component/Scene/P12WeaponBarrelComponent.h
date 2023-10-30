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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12WeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UP12WeaponBarrelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Shot(const FVector& ShotStart, const FVector& ShotDirection, AController* Instigator, const float SpreadAngle);

protected:
	virtual void BeginPlay() override;

private:
	FHitResult HitScan(const FVector& Start, const FVector& End, const FVector& Direction);
	void ProjectileLaunch(const FVector& Start, const FVector& Direction);
	APawn* GetOwningPawn() const;
	AController* GetOwningController() const;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UNiagaraSystem* TraceNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Decal")
	FP12DecalInfo DecalInfo;
};
