// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P16NiagaraComponent.h"
#include "P16PassiveNiagaraComponent.generated.h"

class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API UP16PassiveNiagaraComponent : public UP16NiagaraComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16PassiveNiagaraComponent();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void ListenToChanges(UAbilitySystemComponent* InAbilitySystem) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate);
};
