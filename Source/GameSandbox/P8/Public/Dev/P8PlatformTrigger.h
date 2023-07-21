// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P8PlatformTrigger.generated.h"

class AP8MovablePlatform;
class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AP8PlatformTrigger : public AActor
{
	GENERATED_BODY()

public:
	AP8PlatformTrigger();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnVolumeBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnVolumeEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void MovePlatforms(const bool bMove);
	
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UBoxComponent* PressureVolume;
	UPROPERTY(EditAnywhere, Category = "C++")
	UStaticMeshComponent* PressurePad;
	UPROPERTY(EditAnywhere, Category = "C++")
	TArray<AP8MovablePlatform*> MovablePlatforms;
};
