// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "Character/P16CharacterBase.h"
#include "P16DebuffNiagaraComponent.generated.h"

class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API UP16DebuffNiagaraComponent : public UNiagaraComponent
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
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void ToggleDebuff(const bool bOn);

protected:
	void OnDebuffTagChanged(FGameplayTag CallbackTag, const int32 NewCount);
	void ListenToDebuffTagChanges(UAbilitySystemComponent* AbilitySystemComponent);
	UFUNCTION()
	void OnDeathCallback(AActor* DeadActor = nullptr);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(VisibleAnywhere, Category = "C++")
	FGameplayTag DebuffTag = {};
};
