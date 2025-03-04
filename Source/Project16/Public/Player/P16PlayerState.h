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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD UAttributeSet* GetAttributeSet() const;
	_NODISCARD int32          GetPlayerLevel() const { return Level; }

protected:
	UFUNCTION()
	void OnRep_Level(const int32 OldLevel);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
};
