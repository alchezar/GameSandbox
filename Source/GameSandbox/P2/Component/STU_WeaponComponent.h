// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STU_WeaponComponent.generated.h"

class ASTU_BaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_WeaponComponent();

	void Fire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TSubclassOf<ASTU_BaseWeapon> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName WeaponAttachPointName = "HandWeaponSocket";

private:
	UPROPERTY()
	ASTU_BaseWeapon* CurrentWeapon;

	void SpawnWeapon();
};
