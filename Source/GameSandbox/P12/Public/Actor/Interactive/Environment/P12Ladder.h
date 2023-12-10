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
	FORCEINLINE bool GetIsOnTop() const { return bOnTop; }
	UBoxComponent* GetLadderInteractionBox() const;
	FVector GetAttachFromTopStartLocation() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UStaticMeshComponent* RightRailMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UStaticMeshComponent* LeftRailMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UInstancedStaticMeshComponent* StepRailMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Components")
	UBoxComponent* TopInteractionVolume;

	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float Height = 100.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float Width = 50.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float StepInterval = 58.5f;
	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	float BottomOffset = 46.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Params")
	FVector AttachFromTopInitOffset = FVector(0.f, -20.f, 20.f);

private:
	bool bOnTop = false;
};
