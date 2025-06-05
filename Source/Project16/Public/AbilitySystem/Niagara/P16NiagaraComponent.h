// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "P16NiagaraComponent.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16NiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16NiagaraComponent();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void Init(const FGameplayTag& InTag);
	void ToggleEffect(const bool bActivate);

protected:
	virtual void ListenToChanges(UAbilitySystemComponent* InAbilitySystem);
	UFUNCTION()
	void OnDeathCallback(AActor* DeadActor);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	FGameplayTag Tag = {};
};
