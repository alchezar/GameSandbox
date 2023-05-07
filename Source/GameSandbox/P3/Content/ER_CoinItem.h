// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ER_CoinItem.generated.h"

class USphereComponent;
class URotatingMovementComponent;

UCLASS()
class GAMESANDBOX_API AER_CoinItem : public AActor
{
	GENERATED_BODY()

public:
	AER_CoinItem();
	float GetSpawnProbability();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	USphereComponent* SphereCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	UStaticMeshComponent* CoinMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Component")
	URotatingMovementComponent* RotatingMovement;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Effect")
	USoundBase* OverlapSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Spawn")
	float SpawnProbability = 0.5f;
};
