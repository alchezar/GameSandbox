// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17PickupBase.h"
#include "P17StoneBase.generated.h"

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
};
