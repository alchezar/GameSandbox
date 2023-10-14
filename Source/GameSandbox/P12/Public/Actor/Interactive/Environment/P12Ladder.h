// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Actor/Interactive/P12InteractiveActor.h"
#include "P12Ladder.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class GAMESANDBOX_API AP12Ladder : public AP12InteractiveActor
{
	GENERATED_BODY()

public:
	AP12Ladder();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	FORCEINLINE float GetLadderHeight() const { return Height; }
	UBoxComponent* GetLadderInteractionBox() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UStaticMeshComponent* RightRailMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UStaticMeshComponent* LeftRailMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UInstancedStaticMeshComponent* StepRailMesh;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float Height = 100.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float Width = 50.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float StepInterval = 20.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float BottomOffset = 20.f;
};
