// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG_TerrainTile.generated.h"

class ATG_GameMode;
class UArrowComponent;

UCLASS()
class GAMESANDBOX_API ATG_TerrainTile : public AActor
{
	GENERATED_BODY()

public:
	ATG_TerrainTile();
	virtual void Tick(const float DeltaTime) override;
	FVector      GetAttachPoint();
	bool         GetIsAlwaysUnlocked() const;
	void         SetNextTile(ATG_TerrainTile* NewNextTile);

	void Lock();
	void Unlock();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnLockOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

private:
	UPROPERTY()
	ATG_TerrainTile* NextTile;
	UPROPERTY()
	ATG_GameMode* GameMode;
};
