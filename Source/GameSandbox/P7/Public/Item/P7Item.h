// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P7Item.generated.h"

UCLASS()
class GAMESANDBOX_API AP7Item : public AActor
{
	GENERATED_BODY()

public:
	AP7Item();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* ItemMesh;
};
