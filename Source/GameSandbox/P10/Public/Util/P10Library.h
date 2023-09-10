// Copyright (C) 2023, IKinder

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "P10Library.generated.h"

class AP10Weapon;

UCLASS()
class GAMESANDBOX_API UP10Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void InteractWithPhysical(AActor* CubeActor, UPrimitiveComponent* CubeComp, const AActor* Projectile);

#pragma region DrawDebugs
	
	static bool GetIsDrawDebugAllowed();
	static void DrawDebugShoot(const UObject* WorldContextObject, const FHitResult& Hit);
	static void DrawDebugExplode(const UObject* WorldContextObject, const FVector& HitLocation, const float Radius);
	static void DrawTargetInfo(const UObject* WorldContextObject, const FVector& Location, const FString& Text);
	static void DrawAmmoInfo(const UObject* WorldContextObject, const AP10Weapon* Weapon);
	static FString IntToString(const int32 Number, const int32 DigitCount);
	
#pragma endregion /* DrawDebugs */

#pragma region Bitflags

public:
	
	/**
	 * Add bitflag to current enum state container.\n
	 * 0b0101 = 0b0100
	 *        | 0b0001
	 * @param  State   Current enum state container.
	 * @param  Flag    Flag which we are trying to add.
	 */
	template <class T>
	static void BitflagAdd(T& State, const T Flag)
	{
		State |= Flag;
	}

	/**
	 * Remove bitflag from current enum state container.\n
	 * 0b0100 = 0b0101
	 *        & 0b1110 (~0b0001)
	 * @param  State   Current enum state container.
	 * @param  Flag    Flag which we are trying to remove.
	 */
	template <class T>
	static void BitflagRemove(T& State, const T Flag)
	{
		State &= ~Flag;
	}

	/**
	 * Add or remove bitflag, depending on it`s previous state.\n
	 * 0b0101 = 0b0001
	 *        ^ 0b0100\n
	 * 0b0001 = 0b0101
	 *        ^ 0b0100
	 * @param  State   Current enum state container.
	 * @param  Flag    Flag which we are looking for.
	 */
	template <class T>
	static void BitflagToggle(T& State, const T Flag)
	{
		State ^= Flag;
	}

	/**
	* Add or remove bitflag, depending on the bool condition.\n
	 * @param  State   Current enum state container.
	 * @param  Flag    Flag which we are looking for.
	 * @param  bActive Bool depending on what we are looking.
	 */
	template <class T>
	static void BitflagFromBool(T& State, const T Flag, const bool bActive)
	{
		bActive ? BitflagAdd(State, Flag) : BitflagRemove(State, Flag);
	}

	/**
	 * Check if enum state container has specific flag.\n 
	 * 0b0100 (true)  = 0b0101
	 *                & 0b0100\n
	 * 0b0000 (false) = 0b0001
	 *                & 0b0100
	 * @param  State Current enum state container.
	 * @param  Flag  Flag which we are looking for.
	 * @return       True if the State contains this flag.
	 */
	template <class T>
	static bool BitflagIsActive(const T State, const T Flag)
	{
		return (State & Flag) == Flag;
	}

	/**
	 * Convert enum uint8 value to binary representation and print it as debug message.
	 * @param State  Current enum state container.
	 * @param Length The number of digits we want to print.
	 */
	template <class T>
	static void PrintStateMask(const T State, const int32 Length)
	{
		FString MaskString = "0b";
		uint8 MaskInt = static_cast<uint8>(State);
		int32 Block = FMath::Pow(2.0, Length - 1);
		for (int i = 0; i < Length; ++i)
		{
			MaskString.Append(MaskInt < Block ? "0" : "1");
			MaskInt %= Block;
			Block /= 2;
		}
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("%s"), *MaskString));
	}
#pragma endregion /* Bitflags */
};
