// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P13Types.generated.h"

struct FP13WeaponInfo;
class UNiagaraSystem;
class AP13Weapon;
class AP13ProjectileDefault;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Variables                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const FName WeaponSocketName = "GripPoint";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Enumerations                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UENUM(BlueprintType)
enum class EP13MovementState : uint8
{
	Aim UMETA(DisplayName = "Aim State"),
	Walk UMETA(DisplayName = "Walk State"),
	Run UMETA(DisplayName = "Run State"),
	Sprint UMETA(DisplayName = "Spring State")
};

UENUM(BlueprintType)
enum class EP13AmmoType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Assault rifle"),
	Shotgun UMETA(DisplayName = "ShotGun"),
	Sniper UMETA(DisplayName = "Sniper rifle"),
	Grenade UMETA(DisplayName = "Grenade launcher"),
	Rocket UMETA(DisplayName = "Rocket launcher")
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Structures                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

USTRUCT(BlueprintType)
struct FP13MovementSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Aim = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Walk = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Run = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sprint = 900.f;
};

USTRUCT(BlueprintType)
struct FP13HitEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> Decals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EPhysicalSurface>, UNiagaraSystem*> Particles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Sounds;
};

USTRUCT(BlueprintType)
struct FP13ProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AP13ProjectileDefault> Class = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Lifetime = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitSpeed = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRadialDamage = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13HitEffects OnHit;
};

USTRUCT(BlueprintType)
struct FP13DispersionType
{
	GENERATED_BODY()

	FP13DispersionType() {}

	FP13DispersionType(const float NewMax, const float NewMin, const float NewRecoil, const float NewReduction)
		: Max(NewMax), Min(NewMin), Recoil(NewRecoil), Reduction(NewReduction) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reduction = 0.3f;
};

USTRUCT(BlueprintType)
struct FP13WeaponDispersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13DispersionType Still = {1.f, 0.1f, 1.f, 0.4f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13DispersionType Aim = {2.f, 0.3f, 1.f, 0.3f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13DispersionType Walk = {5.f, 1.f, 1.f, 0.2f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13DispersionType Run = {10.f, 4.f, 1.f, 0.1f};
};

USTRUCT(BlueprintType)
struct FP13WeaponDynamicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Round = 10;
};

USTRUCT(BlueprintType)
struct FP13WeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EP13AmmoType AmmoType = EP13AmmoType::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRound = 0;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FP13WeaponDynamicInfo DynamicInfo;
};

USTRUCT(BlueprintType)
struct FP13AmmoSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EP13AmmoType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount = 100;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Data table                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

USTRUCT(BlueprintType)
struct FP13WeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<AP13Weapon> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EP13AmmoType AmmoType = EP13AmmoType::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Damage = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1.f, ClampMin = 1.f), Category = "State")
	float RateOfFire = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ReloadTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 MaxRound = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 ProjectilesPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	FP13WeaponDispersion WeaponDispersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FP13ProjectileInfo ProjectileSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float DistanceTrace = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ReloadSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* MisfireSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UMaterialInterface* OnHitDecal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FireEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* WeaponFireAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* WeaponReloadAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharFireAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharReloadAnim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharEquipAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* WeaponIcon = nullptr;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                 Class                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13Types : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UP13Types() {};
};
