// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16PlayerCheckpoint.h"
#include "P16MapEntrance.generated.h"

UCLASS()
class PROJECT16_API AP16MapEntrance : public AP16PlayerCheckpoint
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	explicit AP16MapEntrance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @section IP16SaveInterface
	virtual void LoadActor_Implementation() override;

	/// @section IP16HighlightInterface
	virtual EP16TargetingStatus GetTargetingStatus_Implementation() override { return EP16TargetingStatus::MapEntrance; };

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TSoftObjectPtr<UWorld> DestinationMap = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	FName DestinationPlayerStartTag = "";
};
