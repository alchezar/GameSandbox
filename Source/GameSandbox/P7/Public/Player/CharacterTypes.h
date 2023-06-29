#pragma once

#include "CharacterTypes.generated.h"

class AP7Weapon;
class UNiagaraSystem;
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
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking  UMETA(DisplayName = "Attacking"),
	EAS_Equipping  UMETA(DisplayName = "Equipping")
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

	FVector LocationOffset;
	FRotator RotationOffset;
};

USTRUCT(BlueprintType)
struct FNiagaraEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	UNiagaraSystem* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FName ColorName = "RibbonColor";	
};

USTRUCT(BlueprintType)
struct FDecalEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	float Size = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Effect")
	FName ColorName = "Color";
};

USTRUCT(BlueprintType)
struct FWeaponSound
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
