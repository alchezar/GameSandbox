// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15BaseAbility.h"
#include "P15DeathAbility.generated.h"

class AP15Character;

UCLASS()
class PROJECT15_API UP15DeathAbility : public UP15BaseAbility
{
	GENERATED_BODY()
	/* ------------------------------- Super ------------------------------- */
public:
	UP15DeathAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

private:
	void EnableRagdoll() const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Lifespan = 10.f;
};
