// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARAbility.h"
#include "ARAbilitySprint.generated.h"

UCLASS()
class PROJECT06_API UARAbilitySprint : public UARAbility
{
	GENERATED_BODY()

public:
	UARAbilitySprint();
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	virtual void StopAbility_Implementation(AActor* Instigator) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Sprint")
	float BonusSpeed = 600.f;

protected:
	float DefaultSpeed = 0.f;
};
