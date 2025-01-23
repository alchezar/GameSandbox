// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10BlackHole.generated.h"

class USphereComponent;

UCLASS()
class PROJECT10_API AP10BlackHole : public AActor
{
	GENERATED_BODY()

public:
	AP10BlackHole();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

UFUNCTION()
	void OnSphereBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USceneComponent* SceneRootComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* InnerSphereComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* OuterSphereComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UParticleSystem* BlackHoleEffect;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UParticleSystemComponent* ParticleSystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Hole")
	bool bActive = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Hole")
	float OuterSphereRadius = 2000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Hole")
	float AttractionForce = 2000.f;
};
