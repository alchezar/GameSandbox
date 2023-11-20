// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12PickableItem.h"
#include "P12PickableWeapon.generated.h"

UCLASS(Blueprintable)
class GAMESANDBOX_API AP12PickableWeapon : public AP12PickableItem
{
	GENERATED_BODY()

public:
	AP12PickableWeapon();

	virtual void Interact(AP12BaseCharacter* Char) override;
	virtual FName GetActionEventName() const override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* WeaponMesh;
};
