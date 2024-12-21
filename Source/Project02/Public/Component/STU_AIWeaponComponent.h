// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_WeaponComponent.h"
#include "STU_AIWeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT02_API USTU_AIWeaponComponent : public USTU_WeaponComponent
{
	GENERATED_BODY()

public:
	USTU_AIWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;

};
