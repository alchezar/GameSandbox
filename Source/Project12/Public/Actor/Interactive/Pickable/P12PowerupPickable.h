// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12PickableItem.h"
#include "P12PowerupPickable.generated.h"

UCLASS(Blueprintable)
class PROJECT12_API AP12PowerupPickable : public AP12PickableItem
{
	GENERATED_BODY()

public:
	AP12PowerupPickable();

	virtual void Interact(AP12BaseCharacter* Char) override;
	virtual FName GetActionEventName() const override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* PowerupMesh;
};
