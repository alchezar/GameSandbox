// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10LaunchPad.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AP10LaunchPad : public AActor
{
	GENERATED_BODY()

public:
	AP10LaunchPad();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UFUNCTION()
	void OnBoxBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;

public:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USceneComponent* SceneRoot;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* PadMesh;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* BoxCollision;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UArrowComponent* LaunchDirection;

	UPROPERTY(EditAnywhere, Category = "C++")
	float LaunchPower = 500.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	float LaunchAngle = 35.f;
	
private:
	FVector LaunchVelocity = FVector::ZeroVector;
};
