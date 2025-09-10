// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17CombatPawnComponent.h"
#include "P17CombatEnemyComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17CombatEnemyComponent : public UP17CombatPawnComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void OnHitTargetActorCallback(AActor* HitActor) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
private:
	bool GetIsBlocked(AActor* HitActor) const;
};
