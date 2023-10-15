// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12InteractiveActor.generated.h"

class AP12BaseCharacter;

UCLASS(Abstract, NotBlueprintable)
class GAMESANDBOX_API AP12InteractiveActor : public AActor
{
	GENERATED_BODY()

public:
	AP12InteractiveActor();

protected:
	virtual void BeginPlay() override;
	bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp);
	UFUNCTION()
	virtual void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UPrimitiveComponent* InteractionVolume = nullptr;
};
