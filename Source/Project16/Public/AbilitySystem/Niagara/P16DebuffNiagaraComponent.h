// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P16NiagaraComponent.h"
#include "P16DebuffNiagaraComponent.generated.h"

class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API UP16DebuffNiagaraComponent : public UP16NiagaraComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16DebuffNiagaraComponent();

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
	void OnDebuffTagChanged(FGameplayTag CallbackTag, const int32 NewCount);
};
