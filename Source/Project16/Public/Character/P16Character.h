// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "P16CharacterBase.h"
#include "P16Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT16_API AP16Character : public AP16CharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16Character();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void InitAbilityActorInfo() override;

	/* ------------------------------- This -------------------------------- */
public:
	UAttributeSet* GetAttributeSet() const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> Camera = nullptr;
};
