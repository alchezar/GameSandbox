// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P17PickupBase.generated.h"

class USphereComponent;

UCLASS()
class PROJECT17_API AP17PickupBase : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17PickupBase();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	virtual void OnBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USphereComponent> Collision = nullptr;
};
