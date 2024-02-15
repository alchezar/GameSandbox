// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13Respond.h"
#include "GameFramework/Actor.h"
#include "P13SnakeElementBase.generated.h"

class AP13SnakeBase;
class AP13SnakeElementBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnSnakeElementBeginOverlapSignature, AP13SnakeElementBase* /* this */, AActor* /* Overlapped with */)

/* =================================== Class =============================== */
UCLASS()
class GAMESANDBOX_API AP13SnakeElementBase : public AActor, public IP13Respond
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13SnakeElementBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void Respond(AActor* Interactor, const bool bHead) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return SnakeElementMesh; };
	void ToggleCollision();

protected:
	UFUNCTION()
	void OnSnakeElementBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* ------------------------------ Members ------------------------------ */
public:
	FP13OnSnakeElementBeginOverlapSignature OnSnakeElementBeginOverlap;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UStaticMeshComponent* SnakeElementMesh;
};
