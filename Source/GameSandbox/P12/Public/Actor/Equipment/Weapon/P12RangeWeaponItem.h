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
	UP12WeaponBarrelComponent* GetBarrelComponent() const { return WeaponBarrel; }
	void FireInput();

protected:
	virtual void BeginPlay() override;

private:
	float PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate = 1, const FName StartSectionName = NAME_None) const;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP12WeaponBarrelComponent* WeaponBarrel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FName MuzzleSocketName = "Muzzle_Socket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FVector WeaponMeshOffset = FVector(-1.f, -2.f, -3.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FRotator WeaponMeshOrientation = FRotator(20.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Weapon")
	UAnimMontage* WeaponFireMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Character")
	UAnimMontage* CharacterFireMontage;
	

};
