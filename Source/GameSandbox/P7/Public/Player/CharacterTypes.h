#pragma once

#include "CharacterTypes.generated.h"

class AP7Weapon;

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
};

USTRUCT(BlueprintType)
struct FCurrentWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly)
	AP7Weapon* Weapon;
	ECharacterState State;
};