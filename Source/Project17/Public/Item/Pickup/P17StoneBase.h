// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17PickupBase.h"
#include "P17StoneBase.generated.h"

class UGameplayEffect;
class UP17AbilitySystemComponent;

UCLASS()
class PROJECT17_API AP17StoneBase : public AP17PickupBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17StoneBase();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void OnBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void Consume(UP17AbilitySystemComponent* InASC, const int32 InLevel = 1);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnConsumed"))
	void BP_OnConsumed();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;
};
