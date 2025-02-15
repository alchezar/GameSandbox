// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "P15AroundTarget.generated.h"

UCLASS()
class PROJECT15_API AP15AroundTarget : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP15AroundTarget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/* ------------------------------- Super ------------------------------- */
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargeting() override;
	virtual bool IsConfirmTargetingAllowed() override { return bConfirmAllowed; }

	/* ------------------------------- This -------------------------------- */
protected:
	TArray<TWeakObjectPtr<AActor>> GetActorsAround();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Radius = 300.f;

private:
	bool bConfirmAllowed = true;
};
