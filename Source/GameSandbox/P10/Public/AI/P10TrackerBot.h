// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P10TrackerBot.generated.h"

class UFloatingPawnMovement;

UCLASS()
class GAMESANDBOX_API AP10TrackerBot : public APawn
{
	GENERATED_BODY()

public:
	AP10TrackerBot();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void FindNextPathPoint(AActor* Goal);
	void MoveToNextPoint();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float Force = 1000.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float MaxMissCompensation = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	bool bVelocityChange = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float AcceptableDistance = 100.f;
	
private:
	FVector NextPathPoint;
	FVector LastFrameLocation;
	UPROPERTY()
	APawn* TargetPawn;
};
