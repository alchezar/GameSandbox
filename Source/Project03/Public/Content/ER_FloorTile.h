// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_FloorTile.generated.h"

class AER_CoinItem;
class AER_GameModeBase;
class AER_Obstacle;
class UArrowComponent;
class UBoxComponent;

UCLASS()
class PROJECT03_API AER_FloorTile : public AActor
{
	GENERATED_BODY()

public:
	AER_FloorTile();
	virtual void Tick(float DeltaTime) override;

	const FTransform& GetAttachPoint() const;
	TArray<FVector>   GetLaneMidLocations() const;

	void SpawnItems();
	void DestroyFloorTile();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void SetupComponents();
	void SpawnObstacle(const FVector& LaneMidLocation, int& LargeNum, bool& bLarge);
	void SpawnCoin(const FVector& LaneMidLocation, const bool bLarge);
	void SpawnSideCoins(const FVector& LaneMidLocation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UStaticMeshComponent* FloorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UBoxComponent* FloorTriggerBox;

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

private:
	UPROPERTY()
	AER_GameModeBase*     RunGameMode;
	TArray<AER_Obstacle*> Obstacles;
	TArray<AER_CoinItem*> Coins;
};
