// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARAbilityEffect.h"
#include "ARAbilityEffectThorn.generated.h"

UCLASS()
class GAMESANDBOX_API UARAbilityEffectThorn : public UARAbilityEffect
{
	GENERATED_BODY()

public:
	UARAbilityEffectThorn();
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	virtual void StopAbility_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Effect")
	float ReflectFraction = 0.1f;
};
