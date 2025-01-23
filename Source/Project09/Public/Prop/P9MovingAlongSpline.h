// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "P9MovingAlongSpline.generated.h"

struct FTimeline;
class UBoxComponent;
class USplineComponent;
class UCurveFloat;

UCLASS()
class PROJECT09_API AP9MovingAlongSpline : public AActor
{
	GENERATED_BODY()

public:
	AP9MovingAlongSpline();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnTriggerEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void MovementTimelineUpdateHandle(float Output);
	UFUNCTION()
	void MovementTimelineFinishedHandle();
	
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	USplineComponent* SplineComponent;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	UBoxComponent* TriggerComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Timeline")
	UCurveFloat* MovementCurve;
	UPROPERTY(EditAnywhere, Category = "C++ | Timeline")
	bool bAutoActive = false;
	UPROPERTY(EditAnywhere, Category = "C++ | Timeline", meta = (EditCondition = "!bRestartOnFinished"))
	bool bReverseOnFinished = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Timeline", meta = (EditCondition = "!bReverseOnFinished"))
	bool bRestartOnFinished = false;
	
private:
	FTimeline MovementTimeline;
};
