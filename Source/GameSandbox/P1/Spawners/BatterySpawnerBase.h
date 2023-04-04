// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatterySpawnerBase.generated.h"

class UBoxComponent;
class APickUpBase;

UCLASS()
class GAMESANDBOX_API ABatterySpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	ABatterySpawnerBase();
	virtual void Tick(float DeltaTime) override;

	void SetSpawnerActive(const bool bActive);

	UFUNCTION(BlueprintPure, Category = "Kinder")
	FVector GetRandomSpawnPoint() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	USceneComponent* SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	UBillboardComponent* SpriteComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder")
	UBoxComponent* SpawnVolume;
	UPROPERTY(EditAnywhere, Category = "Kinder")
	TSubclassOf<APickUpBase> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	float MinSpawnDelay = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	float MaxSpawnDelay = 5.f;

	FTimerHandle Timer;
	float        RandomSpawnDelay;

private:
	void SpawnActor();
};
