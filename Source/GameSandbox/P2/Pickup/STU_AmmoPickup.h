// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BasePickup.h"
#include "STU_AmmoPickup.generated.h"

class ASTU_BaseWeapon;

UCLASS()
class GAMESANDBOX_API ASTU_AmmoPickup : public ASTU_BasePickup
{
	GENERATED_BODY()

public:
	ASTU_AmmoPickup();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	virtual bool TryToGivePickup(APawn* PlayerPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 Clips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Pickup")
	TSubclassOf<ASTU_BaseWeapon> WeaponType;
};
