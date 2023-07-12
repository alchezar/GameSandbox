#pragma once

#include "CharacterTypes.generated.h"

class AP7Weapon;
class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;

UENUM(BlueprintType)
enum ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHanded  UMETA(DisplayName = "One-Handed"),
	ECS_TwoHanded  UMETA(DisplayName = "Two-Handed")
};

UENUM(BlueprintType)
enum EActionState : uint8
{
	EAS_Unoccupied  UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking   UMETA(DisplayName = "Attacking"),
	EAS_Equipping   UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum EEnemyState : uint8
{
	EES_Dead	   UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing    UMETA(DisplayName = "Chasing"),
	EES_Attaching  UMETA(DisplayName = "Attacking"),
	EES_Engaged    UMETA(DisplayName = "Engaged")
};

UENUM()
enum EJumpState : uint8
{
	EJS_Landed UMETA(DisplayName = "Landed"),
	EJS_Single UMETA(DisplayName = "Single"),
	EJS_Double UMETA(DisplayName = "Double")
};

USTRUCT(BlueprintType)
struct FCurrentWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* Weapon;
	ECharacterState State;
};

USTRUCT(BlueprintType)
struct FSnapOffset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;
	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;
};

USTRUCT(BlueprintType)
struct FP7NiagaraEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UNiagaraSystem* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	FName ColorName = "RibbonColor";	
};

USTRUCT(BlueprintType)
struct FP7DecalEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	float Size = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	FName ColorName = "Color";
};

USTRUCT(BlueprintType)
struct FP7WeaponSound
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USoundBase* Arm;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Disarm;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Hit;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Humming;
};

USTRUCT(BlueprintType)
struct FP7PlasmaSpawn
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;

	FP7PlasmaSpawn()
	{
		Location = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
	}

	explicit FP7PlasmaSpawn(const FHitResult& HitResult)
	{
		Location = HitResult.ImpactPoint;
		Rotation = HitResult.ImpactNormal.Rotation();
	}
};

USTRUCT()
struct FP7PatrolTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Units = "Seconds"))
	float Min = 2.f;
	UPROPERTY(EditAnywhere, meta = (Units = "Seconds"))
	float Max = 5.f;
};
