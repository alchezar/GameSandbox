// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT04_API UGrassComp : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UGrassComp();

protected:
	virtual void BeginPlay() override;

	void SpawnGrass();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Grass")
	FBox SpawningExtends;
	UPROPERTY(EditAnywhere, Category = "Kinder | Grass")
	int32 SpawnCount = 1;
};
