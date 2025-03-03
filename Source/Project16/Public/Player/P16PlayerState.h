// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "P16PlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API AP16PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16PlayerState();

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* ------------------------------- This -------------------------------- */
public:
	UAttributeSet* GetAttributeSet() const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
