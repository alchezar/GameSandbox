#pragma once

#include "STU_CoreTypes.generated.h"

class ASTU_BaseWeapon;

// Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

// Weapon
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTU_BaseWeapon*)

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Weapon")
	TSubclassOf<ASTU_BaseWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Weapon")
	UAnimMontage* ReloadAnimation = nullptr;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Ammo")
	int32 Bullets = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Ammo", meta = (EditCondition = "!bInfinite"))
	int32 Clips = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Ammo")
	bool bInfinite = false;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	UTexture2D* MainIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	UTexture2D* CrossHairIcon;
};
