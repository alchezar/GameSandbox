// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13Respond.h"
#include "GameFramework/Actor.h"
#include "P13Food.generated.h"

UCLASS()
class PROJECT13_API AP13Food : public AActor, public IP13Respond
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13Food();

protected:
	virtual void BeginPlay() override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void Respond(AActor* Interactor, const bool bHead) override;

	/* ------------------------------ Members ------------------------------ */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UStaticMeshComponent* FoodMesh;
};
