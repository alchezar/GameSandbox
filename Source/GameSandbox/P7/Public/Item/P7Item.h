// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P7Item.generated.h"

class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP7Item : public AActor
{
	GENERATED_BODY()

public:
	AP7Item();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	void Oscillation(const float DeltaTime);
	void SetOscillation(const bool bNewSmoothMove);
	
	float TransformedSin();
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	USphereComponent* SphereTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Smooth Moving")
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Smooth Moving")
	float TimeConstant = 5.f;

private:
	float RunningTime = 0.f;
	bool bOscillation = true;
};
