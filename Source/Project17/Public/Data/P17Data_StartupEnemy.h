// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17Data_StartupBase.h"
#include "P17Data_StartupEnemy.generated.h"

class UP17EnemyGameplayAbility;

UCLASS()
class PROJECT17_API UP17Data_StartupEnemy : public UP17Data_StartupBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual void GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel = 1) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Ability")
	TArray<TSubclassOf<UP17EnemyGameplayAbility>> EnemyAbilities = {};
};
