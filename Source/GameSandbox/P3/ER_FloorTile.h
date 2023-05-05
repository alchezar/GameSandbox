// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_FloorTile.generated.h"

class AER_Obstacle;
class UArrowComponent;
class UBoxComponent;
class AER_GameModeBase;

UCLASS()
class GAMESANDBOX_API AER_FloorTile : public AActor
{
	GENERATED_BODY()

public:
	AER_FloorTile();
	virtual void Tick(float DeltaTime) override;
	const FTransform& GetAttachPoint() const;
	TArray<float> GetLaneShiftValues() const;
	void SpawnObstacles();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DestroyFloorTile();
	
private:
	void SetupComponents();
	void SpawnOneObstacle(const float LaneLocation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UStaticMeshComponent* FloorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UBoxComponent* FloorTriggerBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Component")
	TArray<TSubclassOf<AER_Obstacle>> ObstacleClasses;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Point")
	UArrowComponent* AttachPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Point")
	UArrowComponent* CenterLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Point")
	UArrowComponent* LeftLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Point")
	UArrowComponent* RightLane;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Spawn")
	float DestroyDelay = 2.f;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Spawn")
	// float SpawnProbability = 0.5f;

private:
	UPROPERTY()
	AER_GameModeBase* RunGameMode;
	TArray<AER_Obstacle*> Obstacles;
};
