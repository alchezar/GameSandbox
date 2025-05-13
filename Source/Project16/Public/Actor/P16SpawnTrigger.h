// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/P16SaveInterface.h"
#include "P16SpawnTrigger.generated.h"

class AP16SpawnPoint;
class UBoxComponent;

UCLASS()
class PROJECT16_API AP16SpawnTrigger : public AActor, public IP16SaveInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16SpawnTrigger();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual bool GetShouldLoadTransform_Implementation() override { return false; };
	virtual void LoadActor_Implementation() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
protected:
	UFUNCTION()
	void OnTriggerBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UBoxComponent> TriggerVolume = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TArray<AP16SpawnPoint*> Spawners = {};
	UPROPERTY(SaveGame)
	bool bReached = false;
};
