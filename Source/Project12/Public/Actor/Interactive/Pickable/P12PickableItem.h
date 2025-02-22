// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Actor/Interactive/Interface/P12Interactable.h"
#include "GameFramework/Actor.h"
#include "P12PickableItem.generated.h"

UCLASS(Abstract, NotBlueprintable)
class PROJECT12_API AP12PickableItem : public AActor, public IP12Interactable
{
	GENERATED_BODY()

public:
	FORCEINLINE const FName& GetDataTableID() const { return DataTableID; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FName DataTableID = NAME_None;
};
