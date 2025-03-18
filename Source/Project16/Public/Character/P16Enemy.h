// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CharacterBase.h"
#include "Interface/P16InterfaceEnemy.h"
#include "Util/P16Util.h"
#include "P16Enemy.generated.h"

class AP16AIController;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class PROJECT16_API AP16Enemy : public AP16CharacterBase, public IP16InterfaceEnemy
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16Enemy();
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual int32 GetPlayerLevel() override { return Level; };
	virtual void  Die() override;

	virtual void    ToggleHighlight(const bool bOn) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual void    SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
private:
	void InitHealthBar();
	void InitHitReact();

	void OnHitReactCallback(const FGameplayTag Tag, const int32 Count);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP16OnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FP16OnAttributeChangeSignature OnMaxHealthChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UWidgetComponent> HealthBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	EP16CharacterClass CharacterClass = EP16CharacterClass::Warrior;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	float LifeSpan = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI")
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Combat")
	TObjectPtr<AActor> CombatTarget = nullptr;

private:
	UPROPERTY()
	TObjectPtr<AP16AIController> AIController = nullptr;

	bool  bHitReacting  = false;
	float BaseWalkSpeed = 0.f;
};
