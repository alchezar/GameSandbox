// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Util/P16Type.h"
#include "P16EffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class PROJECT16_API AP16EffectActor : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16EffectActor();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void OnBeginOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = "C++")
	void OnEndOverlap(AActor* TargetActor);

private:
	virtual void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& InGameplayEffectClass);
	virtual void RemoveActiveGameplayEffect(AActor* TargetActor);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Applied Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Applied Effect")
	EP16EffectApplicationPolicy ApplicationPolicy = EP16EffectApplicationPolicy::OnBeginOverlap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Applied Effect")
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Infinite Effect")
	bool bInfinite = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Infinite Effect", meta = (EditCondition = "bInfinite", EditConditionHides))
	EP16EffectRemovalPolicy RemovalPolicy = EP16EffectRemovalPolicy::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Infinite Effect", meta = (EditCondition = "bInfinite", EditConditionHides))
	bool bDestroyOnEffectRemoval = false;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectsMap;
};
