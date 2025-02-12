// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "P15GroundSelectTarget.generated.h"

UCLASS()
class PROJECT15_API AP15GroundSelectTarget : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP15GroundSelectTarget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	/* ------------------------------- This -------------------------------- */
public:
	FVector                        GetPlayerLookingPoint(bool* Result = nullptr) const;
	TArray<TWeakObjectPtr<AActor>> GetOverlappedPawns(const FVector& InViewLocation) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Radius = 200.f;
};
