// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "P12PlatformBase.generated.h"

class AP12PlatformTrigger;

UENUM(BlueprintType)
enum class EP12PlatformBehavior : uint8
{
	OnDemand = 0,
	Loop
};

UCLASS()
class PROJECT12_API AP12PlatformBase : public AActor
{
	GENERATED_BODY()

public:
	AP12PlatformBase();
	virtual void Tick(float DeltaTime) override;
	void NewPlatformMove();

protected:
	virtual void BeginPlay() override;

private:
	void SetupTimeline();
	void OnTriggerActiveHandle(bool bActive);
	UFUNCTION()
	void PlatformUpdateHandle(float Output);
	UFUNCTION()
	void PlatformFinishHandle();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UStaticMeshComponent* PlatformMesh;
	UPROPERTY(EditAnywhere, Category = "C++")
	AP12PlatformTrigger* PlatformTrigger;
	UPROPERTY(EditAnywhere, Category = "C++")
	UCurveFloat* MovementCurve;
	UPROPERTY(EditAnywhere, Category = "C++", meta = (MakeEditWidget))
	FVector PlatformEndLocation;
	UPROPERTY(EditAnywhere, Category = "C++", meta = (MakeEditWidget))
	float ReturnTime = 0.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	EP12PlatformBehavior PlatformBehavior;

private:
	FTimeline PlatformTimeline;
	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;
	bool bMovingForward = false;
	FTimerHandle ReturnTimer;
};
