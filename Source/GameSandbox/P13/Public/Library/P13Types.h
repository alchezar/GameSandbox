// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P13Types.generated.h"

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
    Aim    UMETA(DisplayName = "Aim State"),
    Walk   UMETA(DisplayName = "Walk State"),
    Run    UMETA(DisplayName = "Run State"),
    Sprint UMETA(DisplayName = "Spring State")
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
};

USTRUCT(BlueprintType)
struct FP13WeaponDispersion
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FP13WeaponDynamicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Round = 10;
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
    float Damage = 20.f;    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1.f, ClampMin = 1.f), Category = "State")
    float RateOfFire = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ReloadTime = 2.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 MaxRound = 10;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UDecalComponent* DecalOnHit = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* EffectFireWeapon = nullptr;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharReload = nullptr;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CharEquip = nullptr;	
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                 Class                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13Types : public UBlueprintFunctionLibrary
{

GENERATED_BODY()

public:
    UP13Types(){};
};
