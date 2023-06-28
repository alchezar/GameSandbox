// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P7Item.generated.h"

class USphereComponent;

UENUM()
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

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
	void SetItemState(const EItemState NewState);
	
	float TransformedSin();
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	USphereComponent* SphereTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Smooth Moving")
	float Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Smooth Moving")
	float TimeConstant = 5.f;

private:
	float RunningTime = 0.f;
	EItemState ItemState = EItemState::EIS_Hovering;
};
