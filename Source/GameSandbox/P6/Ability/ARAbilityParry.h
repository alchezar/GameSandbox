// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARAbility.h"
#include "ARAbilityParry.generated.h"

UCLASS()
class GAMESANDBOX_API UARAbilityParry : public UARAbility
{
	GENERATED_BODY()

public:
	UARAbilityParry();
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	virtual void StopAbility_Implementation(AActor* Instigator) override;
};


