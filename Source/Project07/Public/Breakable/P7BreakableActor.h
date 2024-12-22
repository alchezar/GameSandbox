// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/P7HitInterface.h"
#include "P7BreakableActor.generated.h"

class UCapsuleComponent;
struct FChaosBreakEvent;
class AP7Treasure;
class UGeometryCollectionComponent;

UCLASS()
class PROJECT07_API AP7BreakableActor : public AActor, public IP7HitInterface
{
	GENERATED_BODY()

public:
	AP7BreakableActor();
	virtual void GetHit(const FVector& HitterLocation) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBreakHandle(const FChaosBreakEvent& BreakEvent);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UCapsuleComponent* CapsulePawnBlocker;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UGeometryCollectionComponent* GeometryCollection;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Sound")
	USoundBase* SmashSound;
	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	TSubclassOf<AP7Treasure> TreasureClass;	
};
