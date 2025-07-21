// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17TraceableAbility.h"
#include "P17TargetLockHeroAbility.generated.h"

class UP17WidgetBase;

struct FP17Neighbours
{
	AActor* Left = nullptr;
	AActor* Right = nullptr;
};

UCLASS()
class PROJECT17_API UP17TargetLockHeroAbility : public UP17TraceableAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17TargetLockHeroAbility();

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
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void LockOnTarget();
	void FindAvailableActorsToLock();
	AActor* GetNearestTarget();
	FP17Neighbours GetNeighbours();

	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	void OrientToTarget(const float DeltaTime) const;
	void FindWidgetSize();

	void ToggleTargetLockMovement(const bool bOn);
	void ToggleTargetLockMappingContext(const bool bOn);

	void CancelTargetLock();
	void CleanUp();

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	TSubclassOf<UP17WidgetBase> WidgetClass = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	float RotationInterpSpeed = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	float MaxWalkSpeed = 150.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++|TargetLock")
	FRotator CameraOffset = {};

	UPROPERTY(EditDefaultsOnly, Category = "C++|Input")
	UInputMappingContext* TargetLockMappingContext = nullptr;

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
