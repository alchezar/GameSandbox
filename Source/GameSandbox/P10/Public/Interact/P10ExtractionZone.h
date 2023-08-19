// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10ExtractionZone.generated.h"

class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AP10ExtractionZone : public AActor
{
	GENERATED_BODY()

public:
	AP10ExtractionZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* OverlapComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UDecalComponent* DecalComponent;	

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Sound")
	USoundBase* MissingSound;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Sound")
	USoundBase* CompleteSound;
};
