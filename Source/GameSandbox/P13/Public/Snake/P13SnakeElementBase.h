// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13SnakeElementBase.generated.h"

UCLASS()
class GAMESANDBOX_API AP13SnakeElementBase : public AActor
{
	GENERATED_BODY()

public:
	AP13SnakeElementBase();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UStaticMeshComponent* GetMesh() { return SnakeElementMesh; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
	UStaticMeshComponent* SnakeElementMesh;
};
