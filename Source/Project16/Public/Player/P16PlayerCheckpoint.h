// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/P16HighlightInterface.h"
#include "Interface/P16SaveInterface.h"
#include "P16PlayerCheckpoint.generated.h"

class USphereComponent;

UCLASS()
class PROJECT16_API AP16PlayerCheckpoint : public APlayerStart, public IP16SaveInterface, public IP16HighlightInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	explicit AP16PlayerCheckpoint(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @section IP16SaveInterface
	virtual bool GetShouldLoadTransform_Implementation() override { return false; };
	virtual void LoadActor_Implementation() override;

	/// @section IP16HighlightInterface
	virtual void                ToggleHighlight_Implementation(const bool bOn) override;
	virtual EP16TargetingStatus GetTargetingStatus_Implementation() override { return EP16TargetingStatus::MapEntrance; };
	virtual void                UpdateDestination_Implementation(FVector& OutDestination) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnCheckpointReached(UMaterialInstanceDynamic* Material);

private:
	void HandleGlowEffects();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	TObjectPtr<USphereComponent> Sphere = nullptr;

	UPROPERTY(SaveGame)
	bool bReached = false;
};
