// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Base/P17HeroGameplayAbility.h"
#include "P17TargetLockHeroAbility.generated.h"

class UP17WidgetBase;

UCLASS()
class PROJECT17_API UP17TargetLockHeroAbility : public UP17HeroGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	/// @par UGameplayAbility interface ----------------------------------------
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(const float DeltaTime);

private:
	void LockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestTarget();

	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	void OrientToTarget(const float DeltaTime) const;
	void FindWidgetSize();

	void ToggleTargetLockMovement(const bool bOn);
	void CancelTargetLock();
	void CleanUp();

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	float TraceDistance = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	FVector TraceBoxSize = FVector {2000.f, 2000.f, 300.f};
	UPROPERTY(EditDefaultsOnly, Category = "C++|Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceChannels = {};

	UPROPERTY(EditDefaultsOnly, Category = "C++|Debug")
	bool bShowDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	TSubclassOf<UP17WidgetBase> WidgetClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	float RotationInterpSpeed = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	float MaxWalkSpeed = 150.f;

	UPROPERTY()
	TArray<AActor*> ActorsToLock = {};
	UPROPERTY()
	AActor* CurrentLockedActor = nullptr;
	UPROPERTY()
	UP17WidgetBase* TargetLockWidget = nullptr;

private:
	FVector2D TargetLockSize = {};
	float DefaultWalkSpeed = 0.f;
};
