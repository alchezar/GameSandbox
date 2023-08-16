// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG_TerrainTile.generated.h"

class UTG_ActorPool;
class ATG_GameMode;
class UArrowComponent;
class UGrassComp;
class ANavMeshBoundsVolume;

USTRUCT(BlueprintType)
struct FNumRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max;

	FNumRange() : Min(0), Max(1)
	{}

	FNumRange(const int32 NewMin, const int32 NewMax)
		: Min(NewMin), Max(NewMax)
	{}
};

USTRUCT(BlueprintType)
struct FSpawnActor
{
	GENERATED_BODY()

	FSpawnActor() : Class(AActor::StaticClass()), Radius(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
};

UCLASS()
class GAMESANDBOX_API ATG_TerrainTile : public AActor
{
	GENERATED_BODY()

public:
	ATG_TerrainTile();
	virtual void Tick(const float DeltaTime) override;

	FVector GetAttachPoint();
	bool    GetIsAlwaysUnlocked() const;
	void    SetNextTile(ATG_TerrainTile* NewNextTile);

	void Lock();
	void Unlock();
	void PlaceCovers();
	void PlaceTroopers(const bool bEnemy);
	void DestroyTile();
	void SetActorPool(UTG_ActorPool* TheActorPool);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnLockOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool FindEmptyLocation(const FVector& Location, const float Radius);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Terrain")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Terrain")
	UStaticMeshComponent* BackWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Terrain")
	UStaticMeshComponent* FrontWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Terrain")
	UArrowComponent* NextTileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Lock")
	UStaticMeshComponent* LockWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Lock")
	UMaterialInterface* LockedAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Lock")
	UMaterialInterface* UnlockedAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Lock")
	bool bAlwaysUnlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	TArray<FSpawnActor> ActorsToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	FNumRange SpawnedActorsNum = FNumRange(5, 10);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	FVector MinExtent = FVector(0.f, -1500.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	FVector MaxExtent = FVector(3000.f, 1500.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI Pawn")
	FSpawnActor PawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI Pawn")
	FNumRange PawnNum = FNumRange(1, 3);

private:
	UPROPERTY()
	ATG_TerrainTile* NextTile;
	UPROPERTY()
	ATG_GameMode* GameMode;
	UPROPERTY()
	TArray<AActor*> CoverActors;
	UPROPERTY()
	UTG_ActorPool* ActorPool;
	UPROPERTY()
	ANavMeshBoundsVolume* NavMeshVolume;
	UPROPERTY()
	TArray<APawn*> Enemies;
	UPROPERTY()
	TArray<APawn*> Friends;
};
