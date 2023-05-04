// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_FloorTile.generated.h"

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

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DestroyFloorTile();
	
private:
	void SetupComponents();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UStaticMeshComponent* FloorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UArrowComponent* AttachPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UArrowComponent* CenterLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UArrowComponent* LeftLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UArrowComponent* RightLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UBoxComponent* FloorTriggerBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Component")
	float DestroyDelay = 2.f;

private:
	UPROPERTY()
	AER_GameModeBase* RunGameMode;
};
