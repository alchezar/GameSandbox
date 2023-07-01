// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7Treasure.generated.h"

UCLASS()
class GAMESANDBOX_API AP7Treasure : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Treasure();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Sound")
	USoundBase* TreasureSound;
};
