// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LS_Slicable.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PROJECT05_API ALS_Slicable : public AActor
{
	GENERATED_BODY()

public:
	ALS_Slicable();
	virtual void Tick(float DeltaTime) override;

	UProceduralMeshComponent* GetSlicableComp() const;
	void Slice(const FVector& PlanePosition, const FVector& PlaneNormal, UMaterialInterface* CapMaterial);

protected:
	virtual void BeginPlay() override;

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	UStaticMeshComponent* ReferenceComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Component")
	UProceduralMeshComponent* SlicableComp;

private:
};
