#pragma once

#include "STU_CoreTypes.generated.h"

class ASTU_BaseWeapon;

// Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

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

// VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	UMaterialInterface* Material;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	FVector Size = FVector(10.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	float Lifetime = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	float FadeOutTime = 1.f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	UNiagaraSystem* NiagaraEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | VFX")
	FDecalData DecalData;
};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Game", meta = (ClampMin = "1", ClampMax = "16"))
	int32 PlayersNumber = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Game", meta = (ClampMin = "1", ClampMax = "8"))
	int32 RoundsNum = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Game", meta = (ClampMin = "8", ClampMax = "256", Units = "s"))
	int32 RoundTime = 16;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Game", meta = (ClampMin = "1", ClampMax = "20", Units = "s"))
	int32 RespawnTime = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Team")
	FLinearColor DefaultTeamColor = FLinearColor::White;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Team")
	TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Team")
	FLinearColor DefaultBlasterColor = FLinearColor(10.f, 0.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Team")
	TArray<FLinearColor> BlasterColors;
};
