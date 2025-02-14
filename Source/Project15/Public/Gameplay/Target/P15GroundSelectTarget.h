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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* ------------------------------- Super ------------------------------- */
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	/* ------------------------------- This -------------------------------- */
public:
	FVector                        GetPlayerLookingPoint(bool* Result = nullptr) const;
	TArray<TWeakObjectPtr<AActor>> GetOverlappedPawns(const FVector& InViewLocation) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UDecalComponent> Decal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Radius = 200.f;
};
