// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Actor/Equipment/P12EquipableItem.h"
#include "P12MeleeWeaponItem.generated.h"

USTRUCT(BlueprintType)
struct FP12MeleeAttackDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float Damage = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage;
};

UCLASS(Blueprintable)
class GAMESANDBOX_API AP12MeleeWeaponItem : public AP12EquipableItem
{
	GENERATED_BODY()

public:
	AP12MeleeWeaponItem();
	virtual void Tick(const float DeltaTime) override;
	void StartAttack(EP12MeleeAttackType AttackType);
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAnimNotify() override;
	void OnMeleeAttackStartHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void OnMeleeAttackEndHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void OnEquipmentStateChangedHandle(const bool bEquipped);

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Melee")
	TMap<EP12MeleeAttackType, FP12MeleeAttackDescription> Attacks;

private:
	FP12MeleeAttackDescription* CurrentAttack;
};
