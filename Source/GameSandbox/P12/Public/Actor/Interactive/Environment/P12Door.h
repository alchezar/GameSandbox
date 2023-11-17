// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "P12/Public/Actor/Interactive/Interface/P12Interactable.h"
#include "P12Door.generated.h"

struct FTimeline;

USTRUCT(BlueprintType)
struct FP12DoorAngle
{
	GENERATED_BODY()

	FP12DoorAngle()
		{}
	FP12DoorAngle(const float NewClosed, const float NewOpened)
		: Closed(NewClosed), Opened(NewOpened) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Closed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Opened = 90.f;
};

UCLASS()
class GAMESANDBOX_API AP12Door : public AActor, public IP12Interactable
{
	GENERATED_BODY()

public:
	AP12Door();
	virtual void Tick(const float DeltaTime) override;
	virtual void Interact(AP12BaseCharacter* Char) override;
	virtual FName GetActionEventName() const override;

protected:
	virtual void BeginPlay() override;

private:
	void InteractWithDoor();
	UFUNCTION()
	void OnUpdateDoorTimelineHandle(float TimelineAlpha);
	UFUNCTION()
	void OnFinishedDoorTimelineHandle();

	void OpenDoor();
	void OpenDoorSmoothly(FRotator StartRotation, FRotator TargetRotation);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* DoorPivot;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* DoorMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(EditAnywhere, Category = "C++ | Door opening")
	FP12DoorAngle Angle;
	UPROPERTY(EditAnywhere, Category = "C++ | Door opening")
	bool bCustomCurve = false;
	UPROPERTY(EditAnywhere, Category = "C++ | Door opening", meta = (EditCondition = "bCustomCurve"))
	UCurveFloat* DoorCurve;
	UPROPERTY(EditAnywhere, Category = "C++ | Door opening", meta = (EditCondition = "!bCustomCurve", ClampMin = 0.01f, UIMin = 0.01f))
	float OpeningTime = 1.f;

private:
	bool bOpened = false;
	FTimeline DoorOpenTimeline;

	FTimerHandle DoorTimer;
	float Alpha = 1.f;
};
