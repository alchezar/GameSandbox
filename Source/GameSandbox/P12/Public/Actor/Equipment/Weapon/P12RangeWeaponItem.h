// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Actor/Equipment/P12EquipableItem.h"
#include "P12RangeWeaponItem.generated.h"

class UP12WeaponBarrelComponent;

UCLASS(Blueprintable)
class GAMESANDBOX_API AP12RangeWeaponItem : public AP12EquipableItem
{
	GENERATED_BODY()

public:
	AP12RangeWeaponItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP12WeaponBarrelComponent* WeaponBarrel;

	UPROPERTY(VisibleAnywhere, Category = "C++ | Alignment")
	FName MuzzleSocketName = "Muzzle_Socket";
	UPROPERTY(VisibleAnywhere, Category = "C++ | Alignment")
	FVector WeaponMeshOffset = FVector(0.f, 0.f, 0.f);
};
