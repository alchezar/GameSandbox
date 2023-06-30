// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P7/Public/Interface/P7HitInterface.h"
#include "P7BreakableActor.generated.h"

struct FChaosBreakEvent;
class UGeometryCollectionComponent;

UCLASS()
class GAMESANDBOX_API AP7BreakableActor : public AActor, public IP7HitInterface
{
	GENERATED_BODY()

public:
	AP7BreakableActor();
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBreakHandle(const FChaosBreakEvent& BreakEvent);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UGeometryCollectionComponent* GeometryCollection;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Sound")
	USoundBase* SmashSound;
};
