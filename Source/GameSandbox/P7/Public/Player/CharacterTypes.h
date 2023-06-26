#pragma once

UENUM(BlueprintType)
enum ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHanded  UMETA(DisplayName = "One-Handed"),
	ECS_TwoHanded  UMETA(DisplayName = "Two-Handed")
};