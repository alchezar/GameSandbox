// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "P16PlayerCheckpoint.generated.h"

class USphereComponent;

UCLASS()
class PROJECT16_API AP16PlayerCheckpoint : public APlayerStart
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
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnCheckpointReached(UMaterialInstanceDynamic* Material);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	TObjectPtr<USphereComponent> Sphere = nullptr;
};
