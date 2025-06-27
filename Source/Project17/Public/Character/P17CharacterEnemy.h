// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17CharacterBase.h"
#include "P17CharacterEnemy.generated.h"

struct FStreamableHandle;
class UP17CombatEnemyComponent;

UCLASS()
class PROJECT17_API AP17CharacterEnemy : public AP17CharacterBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17CharacterEnemy();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @par IP17CombatInterface -----------------------------------------------
	virtual UP17CombatPawnComponent* GetCombatComponent() const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UP17CombatEnemyComponent* GetEnemyCombatComponent() const { return CombatComponent; };

private:
	void InitEnemyStartupData() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17CombatEnemyComponent> CombatComponent = nullptr;
};
