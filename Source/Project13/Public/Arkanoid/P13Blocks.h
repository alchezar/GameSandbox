// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13Blocks.generated.h"

UCLASS()
class PROJECT13_API AP13Blocks : public AActor
{
	GENERATED_BODY()

public:
	AP13Blocks();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;

private:
	void PlaceBlocks();
	void OnBallHitHandle(UPrimitiveComponent* MyComp, AActor* Other);

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Block")
	UStaticMesh* BlockMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "C++ | Block")
	UMaterialInstance* BlockMaterial;

	UPROPERTY(EditAnywhere, Category = "C++ | Params", meta = (UIMin = 1, ClampMin = 1))
	int32 NumX = 10;
	UPROPERTY(EditAnywhere, Category = "C++ | Params", meta = (UIMin = 1, ClampMin = 1))
	int32 NumY = 10;
	UPROPERTY(EditAnywhere, Category = "C++ | Params", meta = (UIMin = 1, ClampMin = 1))
	int32 Padding = 120;

private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> SpawnedBlocks;
};
