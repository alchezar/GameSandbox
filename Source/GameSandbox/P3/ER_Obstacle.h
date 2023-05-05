// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_Obstacle.generated.h"

class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AER_Obstacle : public AActor
{
	GENERATED_BODY()

public:
	AER_Obstacle();
	float GetSpawnProbability() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Components")
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Components")
	UBoxComponent* KillTrigger;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Spawn")
	float SpawnProbability = 0.5f;
};
